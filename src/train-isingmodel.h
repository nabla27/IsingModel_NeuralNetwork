
#ifndef TRAINISINGMODEL_H
#define TRAINISINGMODEL_H

#ifndef NDEBUG
#define NDEBUG
#endif

//#ifdef NDEBUG
//#undef NDEBUG
//#endif

#include "neuralnetwork.h"
#include "isingmodel.h"

/* スピン配位の学習データを熱浴法で作成し，保存する．
 * 転移温度前後で異なるラベル付けをする．
 */
void createIsingModelDataSet()
{
    using namespace nn;

    vec2d train_x; //学習データ
    vec2d train_t; //学習ラベル
    vec2d test_x;  //テストデータ
    vec2d test_t;  //テストラベル

    IsingModel ising;
    State<20, 20, bool> state;
    IsingHeatBathMethod hbMethod(&ising);
    const int halfDataCount = 10; //作成するデータ数の半分
    const double t = 2 * ising.param.J / (ising.param.kb * std::log(std::sqrt(2) + 1)); //相転移温度

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> lt(0, t);     //[0,転移温度]の一様な乱数
    std::uniform_real_distribution<> ht(t, 2 * t); //[転移温度,2*転移温度]の一様な乱数

    /* 転移温度より低い温度でのスピン配位を熱浴法で作成 */
    for(int i = 0; i < halfDataCount * 2; ++i)
    {
        ising.param.T = lt(mt);
        std::cout << i << '\t' << ising.param.T << std::endl;

        state.initRand();
        hbMethod.optimize<1000000>(state);

        vec1d data;
        state.createVector1d<double>(data);

        if(i < halfDataCount)
        {
            //学習データに加える
            train_x.push_back(data);
            train_t.push_back({ 0, 1 });
        }
        else
        {
            //テストデータに加える
            test_x.push_back(data);
            test_t.push_back({ 0, 1 });
        }
    }
    /* 転移温度より高い温度でのスピン配位を熱浴法で作成 */
    for(int i = 0; i < halfDataCount * 2; ++i)
    {
        ising.param.T = ht(mt);
        std::cout << i << '\t' << ising.param.T << std::endl;

        state.initRand();
        hbMethod.optimize<1000000>(state);

        vec1d data;
        state.createVector1d<double>(data);

        if(i < halfDataCount)
        {
            //学習データに加える
            train_x.push_back(data);
            train_t.push_back({ 1, 0 });
        }
        else
        {
            //テストデータに加える
            test_x.push_back(data);
            test_t.push_back({ 1, 0 });
        }
    }

    /* 作成したスピン配位を保存 */
    const std::string folder = "F:/repos/isingdata/7_rand/";
    IOVector::writeVec2d(train_x, folder + "train_x.txt");
    IOVector::writeVec2d(train_t, folder + "train_t.txt");
    IOVector::writeVec2d(test_x, folder + "test_x.txt");
    IOVector::writeVec2d(test_t, folder + "test_t.txt");
}






/* 保存されたスピン配位の学習データをよみとり，ニューラルネットワークで学習させる．
 * 学習はすぐに収束するので，そのまま学習済みのパラメータを用いて各温度のスピン配位の磁化を推論する．
 */
void predictMagnetization()
{
    using namespace nn;

    /* 保存しているスピン配位の学習データを読み取る */
    const std::string folder = "F:/repos/isingdata/6_rand/";
    const vec2d train_x = IOVector::readVec2d(folder + "train_x.txt");
    const vec2d train_t = IOVector::readVec2d(folder + "train_t.txt");
    const vec2d test_x = IOVector::readVec2d(folder + "test_x.txt");
    const vec2d test_t = IOVector::readVec2d(folder + "test_t.txt");

    NetworkModel nModel(train_x[0].size(), train_t[0].size());
    LearningModel lModel(nModel);
    Network network(lModel);

    lModel.setBatchSize(20);                 //バッチサイズ
    lModel.setStepCount(1e6);                //最大学習ステップ数
    lModel.setTrainData(&train_x, &train_t); //学習データをセット
    lModel.setTestData(&test_x, &test_t);    //テストデータをセット

    /* レイヤの追加 */
    nModel.addLayer(Layer::LayerType::AffineLayer, 10);
    nModel.addLayer(Layer::LayerType::BatchNormLayer);
    nModel.addLayer(Layer::LayerType::TanhExpLayer);
    nModel.addLayer(Layer::LayerType::DropOutLayer);
    nModel.addLayer(Layer::LayerType::AffineLayer, 2);
    nModel.addLayer(Layer::LayerType::BatchNormLayer);
    nModel.addLayer(Layer::LayerType::SoftmaxLayer);

    /* 学習状況を確認する関数 */
    struct Observer {
        static void func(Network::LearningInfo& info, const LearningModel& model)
        {
            if(info.step % info.numIter != 0) return;

            /* 損失関数 */
            const double loss = Network::loss(*(info.out), *(info.batch_t));
            /* 学習データの精度 */
            const double acc_train = Network::accuracy(model.networkModel(), model.get_train_x(), model.get_train_t());
            /* テストデータの精度 */
            const double acc_test = Network::accuracy(model.networkModel(), model.get_test_x(), model.get_test_t());

            std::cout << "step:" << info.step << '\t'
                      << "epoch:" << info.epoch << '\t'
                      << "loss:" << loss << '\t'
                      << "train-acc:" << acc_train << '\t'
                      << "test-acc:" << acc_test << std::endl;

            /* 70エポックで学習を終了 */
            if(info.epoch > 70) info.breakFlag = true;
        }};

    network.setObserver(&Observer::func);

    /* 学習する */
    network.train();

    vec2d x;
    State<20, 20, bool> state;
    IsingModel ising;
    IsingHeatBathMethod<LatticeType::Hexagonal> hbMethod(&ising);

    int maxCount = 20;
    vec2d mdata;

    /* 推論用の各温度のスピン配位を熱浴法で作成 */
    for(int count = 1; count <= maxCount; ++count)
    {
        const double tStride = 0.01;
        double T = 0.0;
        while(T < 10.0)
        {
            ising.param.T = T;

            state.initRand();

            hbMethod.optimize<1000000>(state);

            vec1d vec;
            state.createVector1d<double>(vec);
            x.push_back(vec);

            std::cout << count << ", " << "T:" << T << std::endl;

            T += tStride;
        }

        /* 学習済みのネットワークに各温度のスピン配位を渡して出力を得る */
        const vec2d out = Network::forward(nModel, x);

        /* 温度と出力を保存 */
        mdata.resize(out.size(), vec1d(3));
        for(size_t i = 0; i < out.size(); ++i)
        {
            mdata[i][0] = i * tStride;
            mdata[i][1] += out[i][0];
            mdata[i][2] += out[i][1];
        }

        x.clear();
    }

    std::ofstream fout;
    fout.open("F:/repos/CmpPhys2/03/geditor/train_log/m_hexagonal.csv");
    for(size_t i = 0; i < mdata.size(); ++i)
    {
        fout << mdata[i][0] << ',' << mdata[i][1] / maxCount << ',' << mdata[i][2] / maxCount << '\n';
    }
}

#endif // TRAINISINGMODEL_H
