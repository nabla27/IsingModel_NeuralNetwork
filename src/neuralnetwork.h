#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <random>
#include <cassert>
#include <unordered_set>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>


namespace nn
{

using vec1d = std::vector<double>;
using vec2d = std::vector<vec1d>;
using vec3d = std::vector<vec2d>;

inline void print(const vec2d& vec)
{
    const size_t rows = vec.size();
    const size_t cols = (rows > 0) ? vec[0].size() : 0;

    std::cout << "----------" << rows << ',' << cols << "----------" << std::endl;

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            std::cout << vec[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}

class IOVector
{
public:
    enum class Format { Csv };

    template<Format = Format::Csv>
    static vec2d readVec2d(const std::string& path);
    template<>
    static vec2d readVec2d<Format::Csv>(const std::string& path)
    {
        std::ifstream fin;
        fin.open(path, std::ios::in);

        std::string buffer;
        if(!std::getline(fin, buffer)) return vec2d();

        std::vector<std::string> info;
        {
            std::string str;
            for(const char& c : buffer)
            {
                if(c == ',')
                {
                    info.push_back(str);
                    str.clear();
                }
                else
                    str += c;
            }
            if(!str.empty()) info.push_back(str);
        }

        if(info.size() < 2) return vec2d();

        const int rows = std::stoi(info[0]);
        const int cols = std::stoi(info[1]);
        vec2d data(rows, vec1d(cols));

        if(!std::getline(fin, buffer)) return vec2d();

        int i = 0;
        int j = 0;
        std::string str;
        for(const char& c : buffer)
        {
            if(c == ',')
            {
                data[i][j] = std::stod(str);
                str.clear();

                j = (++j) % cols;
                if(j == 0) i ++;

                if(i >= rows || j >= cols) break;
            }
            else
                str += c;
        }

        return data;
    }

    template<Format = Format::Csv>
    static void writeVec2d(const vec2d& vec, const std::string& path);
    template<>
    static void writeVec2d<Format::Csv>(const vec2d& vec, const std::string& path)
    {
        std::ofstream fout;
        fout.open(path, std::ios::out);

        const size_t rows = vec.size();
        const size_t cols = vec[0].size();

        fout << rows << ',' << cols << '\n';

        for(const auto& i : vec)
            for(const auto& j : i)
                fout << j << ',';

        fout << '\n';
    }
};

struct PropagationInfo
{
    bool isTraining = true;
};

class Layer
{
public:
    Layer(const size_t forwardOutSize, const size_t backwardOutSize)
        : _dataCount(1)
        , _forwardOutSize(forwardOutSize)
        , _backwardOutSize(backwardOutSize)
        , forwardOut(_dataCount, vec1d(_forwardOutSize))
        , backwardOut(_dataCount, vec1d(_backwardOutSize)) {}

    virtual ~Layer() {}

    enum class LayerType { AffineLayer,
                           ReLULayer,
                           SigmoidLayer,
                           TanhExpLayer,
                           DropOutLayer,
                           BatchNormLayer,
                           SoftmaxLayer,
                         };

    virtual const vec2d *const forward(const vec2d *const in, PropagationInfo& info) = 0;
    virtual const vec2d *const backward(const vec2d *const in, PropagationInfo& info) = 0;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void reset() = 0;

    virtual void setDataCount(const size_t& dataCount)
    {
        forwardOut.resize(dataCount, vec1d(_forwardOutSize));
        backwardOut.resize(dataCount, vec1d(_backwardOutSize));
        _dataCount = dataCount;
    }

    size_t dataCount() const { return _dataCount; }
    size_t forwardOutSize() const { return _forwardOutSize; }
    size_t backwardOutSize() const { return _backwardOutSize; }

protected:
    size_t _dataCount;
    const size_t _forwardOutSize;
    const size_t _backwardOutSize;

    vec2d forwardOut;
    vec2d backwardOut;
};

class AffineLayer : public Layer
{
public:
    AffineLayer(const size_t numNodes, const size_t numPrevNodes)
        : Layer(numNodes, numPrevNodes)

        , W(numPrevNodes, vec1d(numNodes, 0))
        , b(numNodes)
        , dW(numPrevNodes, vec1d(numNodes, 0))
        , db(numNodes)
        , hW(numPrevNodes, vec1d(numNodes, 1e-7))
        , hb(numNodes, 1e-7) {}

    const vec2d *const forward(const vec2d *const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        x = in;

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
            {
                for(size_t k = 0; k < _backwardOutSize; ++k)
                {
                    forwardOut[i][j] += (*in)[i][k] * W[k][j];
                }
                forwardOut[i][j] += b[j];
            }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d *const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _forwardOutSize);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
                for(size_t k = 0; k < _forwardOutSize; ++k)
                {
                    backwardOut[i][j] += (*in)[i][k] * W[j][k];
                }

        for(size_t i = 0; i < _backwardOutSize; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
            {
                for(size_t k = 0; k < _dataCount; ++k)
                {
                    dW[i][j] += (*x)[k][i] * (*in)[k][j];
                    db[j] += (*in)[k][j];
                }
            }

        return &backwardOut;
    }
    void init() override
    {
        static std::random_device rnd;
        static std::mt19937 mt(rnd());
        static std::normal_distribution<> dist(0.0, 1 / sqrt(_backwardOutSize));

        for(size_t i = 0; i < _forwardOutSize; ++i)
        {
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                W[j][i] = dist(mt);
            }
            b[i] = dist(mt);
        }
    }
    void update() override
    {
        static const double lr = 0.1;
        static const double eps = 1e-7;

        for(size_t i = 0; i < _forwardOutSize; ++i)
        {
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                assert(hW[j][i] + eps > 0.0);

                hW[j][i] += dW[j][i] * dW[j][i];
                W[j][i] -= lr * (1.0 / std::sqrt(hW[j][i] + eps)) * dW[j][i];
            }

            assert(hb[i] + eps > 0.0);

            hb[i] += db[i] * db[i];
            b[i] -= lr * (1.0 / std::sqrt(hb[i] + eps)) * db[i];
        }
    }
    void reset() override
    {
        for(size_t i = 0; i < _forwardOutSize; ++i)
        {
            for(size_t j = 0; j < _backwardOutSize; ++j)
                dW[j][i] = 0.0;
            db[i] = 0.0;
        }
        for(size_t i = 0; i < _dataCount; ++i)
        {
            for(size_t j = 0; j < _forwardOutSize; ++j)
                forwardOut[i][j] = 0.0;
            for(size_t j = 0; j < _backwardOutSize; ++j)
                backwardOut[i][j] = 0.0;
        }
    }

public:
    const vec2d* x;

    vec2d W;
    vec1d b;
    vec2d dW;
    vec1d db;

    vec2d hW;
    vec1d hb;
};

class ReLULayer : public Layer
{
public:
    ReLULayer(const size_t numPrevNodes)
        : Layer(numPrevNodes, numPrevNodes) {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        x = in;

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                const auto val = (*in)[i][j];
                forwardOut[i][j] = (val <= 0) ? 0 : val;
            }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                backwardOut[i][j] = ((*x)[i][j] <= 0) ? 0 : (*in)[i][j];
            }

        return &backwardOut;
    }
    void init() override {}
    void update() override {}
    void reset() override {}

private:
    const vec2d* x;
};

class SigmoidLayer : public Layer
{
public:
    SigmoidLayer(const size_t numPrevNodes)
        : Layer(numPrevNodes, numPrevNodes) {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                forwardOut[i][j] = 1.0 / (1.0 + std::exp(-(*in)[i][j]));
            }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
                backwardOut[i][j] = (*in)[i][j] * (1.0 - forwardOut[i][j]) * forwardOut[i][j];

        return &backwardOut;
    }
    void init() override {}
    void update() override {}
    void reset() override {}
};

class TanhExpLayer : public Layer
{
public:
    TanhExpLayer(const size_t numPrevNodes)
        : Layer(numPrevNodes, numPrevNodes) {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo &) override
    {
        assert(_dataCount == (*in).size());
        assert(_backwardOutSize == (*in)[0].size());

        mask = in;

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                const auto value = (*in)[i][j];

                if(value > 3) forwardOut[i][j] = value;
                else if(value < -25) forwardOut[i][j] = 0;
                else forwardOut[i][j] = value * std::tanh(std::exp(value));
            }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo &) override
    {
        assert(_dataCount == (*in).size());
        assert(_forwardOutSize == (*in)[0].size());

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
            {
                const auto m = (*mask)[i][j];

                if(m > 3) backwardOut[i][j] = (*in)[i][j];
                else if(m < -25) backwardOut[i][j] = 0;
                else
                {
                    const double tanhExp = std::tanh(std::exp(m));
                    backwardOut[i][j] = (*in)[i][j] * (tanhExp - m * std::exp(m) * (tanhExp * tanhExp - 1));
                }
            }

        return &backwardOut;
    }
    void init() override {}
    void update() override {}
    void reset() override {}

private:
    const vec2d *mask;
};

class DropOutLayer : public Layer
{
public:
    DropOutLayer(const size_t numPrevNodes, const double ratio = 0.15)
        : Layer(numPrevNodes, numPrevNodes)
        , ratio(ratio)
        , mask(_dataCount, vec1d(_backwardOutSize, 0)) {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo& info) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        if(info.isTraining)
        {
            static std::random_device rnd;
            static std::mt19937 mt(rnd());
            static std::uniform_real_distribution<> rand(0, 1);

            for(size_t i = 0; i < _dataCount; ++i)
                for(size_t j = 0; j < _backwardOutSize; ++j)
                {
                    //割合(ratio)でニューロンを消す
                    if(rand(mt) > ratio)
                    {
                        mask[i][j] = 1.0;
                        forwardOut[i][j] = (*in)[i][j];
                    }
                    else
                    {
                        mask[i][j] = 0.0;
                        forwardOut[i][j] = 0.0;
                    }
                }
        }
        else
        {
            for(size_t i = 0; i < _dataCount; ++i)
                for(size_t j = 0; j < _backwardOutSize; ++j)
                {
                    forwardOut[i][j] = (*in)[i][j] * (1.0 - ratio);
                }
        }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo& info) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);
        assert(info.isTraining);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                backwardOut[i][j] = (*in)[i][j] * mask[i][j];
            }

        return &backwardOut;
    }
    void init() override {}
    void update() override {}
    void reset() override {}
    void setDataCount(const size_t& dataCount) override
    {
        mask.resize(dataCount, vec1d(_backwardOutSize));
        Layer::setDataCount(dataCount);
    }

    void setRatio(const double ratio) noexcept { this->ratio = ratio; }

private:
    double ratio;
    vec2d mask;
};

class BatchNormLayer : public Layer
{
public:
    BatchNormLayer(const size_t numPrevNodes)
        : Layer(numPrevNodes, numPrevNodes)
        , gamma(_backwardOutSize, 1.0)
        , beta(_backwardOutSize, 0.0)
        , eta(0.9)
        , meanMemory(_backwardOutSize)
        , varianceMemory(_backwardOutSize)
        , xc(_dataCount, vec1d(_backwardOutSize))
        , xn(_dataCount, vec1d(_backwardOutSize))
        , std(_backwardOutSize)
        , dgamma(_backwardOutSize)
        , dbeta(_backwardOutSize)
    {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo &info) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        if(info.isTraining)
        {
            vec1d mean(_backwardOutSize, 0.0);
            vec1d variance(_backwardOutSize, 0.0);

            /* 平均を求める */
            for(size_t i = 0; i < _backwardOutSize; ++i)
            {
                for(size_t j = 0; j < _dataCount; ++j)
                {
                    mean[i] += (*in)[j][i];
                }
                mean[i] /= _dataCount;
            }
            /* 偏差 */
            for(size_t i = 0; i < _dataCount; ++i)
                for(size_t j = 0; j < _backwardOutSize; ++j)
                    xc[i][j] = (*in)[i][j] - mean[j];
            /* 分散 */
            for(size_t i = 0; i < _backwardOutSize; ++i)
            {
                for(size_t j = 0; j < _dataCount; ++j)
                {
                    variance[i] += xc[j][i] * xc[j][i];
                }
                variance[i] /= _dataCount;
            }
            /* 標準偏差 */
            for(size_t i = 0; i < _backwardOutSize; ++i)
                std[i] = std::sqrt(variance[i] + 1e-7);
            /* 標準化 */
            for(size_t i = 0; i < _dataCount; ++i)
                for(size_t j = 0; j < _backwardOutSize; ++j)
                    xn[i][j] = xc[i][j] / std[j];

            for(size_t i = 0; i < _backwardOutSize; ++i)
            {
                meanMemory[i] = eta * meanMemory[i] + (1.0 - eta) * mean[i];
                varianceMemory[i] = eta * varianceMemory[i] + (1.0 - eta) * variance[i];
            }
        }
        else
        {
            for(size_t i = 0; i < _dataCount; ++i)
                for(size_t j = 0; j < _backwardOutSize; ++j)
                {
                    xc[i][j] = (*in)[i][j] - meanMemory[j];
                    xn[i][j] = xc[i][j] / std::sqrt(varianceMemory[j] + 1e-7);;
                }
        }

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
                forwardOut[i][j] = gamma[j] * xn[i][j] + beta[j];

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _forwardOutSize);

        for(size_t i = 0; i < _backwardOutSize; ++i)
            for(size_t j = 0; j < _dataCount; ++j)
            {
                dbeta[i] += (*in)[j][i];
                dgamma[i] += xn[j][i] * (*in)[j][i];
            }

        vec2d dxn(_dataCount, vec1d(_forwardOutSize));
        vec2d dxc(_dataCount, vec1d(_forwardOutSize));
        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
            {
                dxn[i][j] = gamma[j] * (*in)[i][j];
                dxc[i][j] = dxn[i][j] / std[j];
            }

        vec1d dstd(_forwardOutSize);
        for(size_t i = 0; i < _forwardOutSize; ++i)
            for(size_t j = 0; j < _dataCount; ++j)
            {
                dstd[i] += - (dxn[j][i] * xc[j][i]) / (std[i] * std[i]);
            }

        vec1d dvar(_forwardOutSize);
        for(size_t i = 0; i < _forwardOutSize; ++i)
            dvar[i] = 0.5 * dstd[i] / std[i];

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
                dxc[i][j] += (2.0 / _dataCount) * xc[i][j] * dvar[j];

        vec1d dmu(_forwardOutSize);
        for(size_t i = 0; i < _forwardOutSize; ++i)
            for(size_t j = 0; j < _dataCount; ++j)
                dmu[i] += dxc[j][i];

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _forwardOutSize; ++j)
                backwardOut[i][j] = dxc[i][j] - dmu[j] / _dataCount;

        return &backwardOut;
    }
    void init() override {}
    void update() override
    {
        static const double lr = 0.01;
        for(size_t i = 0; i < _backwardOutSize; ++i)
        {
            beta[i] -= lr * dbeta[i];
            gamma[i] -= lr * dgamma[i];
        }
    }
    void reset() override
    {
        std::fill(dgamma.begin(), dgamma.end(), 0);
        std::fill(dbeta.begin(), dbeta.end(), 0);
    }
    void setDataCount(const size_t& dataCount) override
    {
        xc.resize(dataCount, vec1d(_backwardOutSize));
        xn.resize(dataCount, vec1d(_backwardOutSize));
        Layer::setDataCount(dataCount);
    }

private:
    vec1d gamma;
    vec1d beta;
    double eta;

    vec1d meanMemory;
    vec1d varianceMemory;

    vec2d xc;
    vec2d xn;
    vec1d std;
    vec1d dgamma;
    vec1d dbeta;
};

class SoftMaxLayer : public Layer
{
public:
    SoftMaxLayer(const size_t numPrevNodes)
        : Layer(numPrevNodes, numPrevNodes) {}

    const vec2d *const forward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);

        for(size_t i = 0; i < _dataCount; ++i)
        {
            double max = - DBL_MAX; //その行の最大値

            /* 行の最大要素を見つける */
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                if(max < (*in)[i][j]) max = (*in)[i][j];
            }

            double deno = 0.0; //行のexp(in-max)の和

            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                deno += exp((*in)[i][j] - max);
            }
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                assert(deno + 1e-7 != 0);
                forwardOut[i][j] = std::exp((*in)[i][j] - max) / (deno + 1e-7);
            }
        }

        return &forwardOut;
    }
    const vec2d *const backward(const vec2d * const in, PropagationInfo&) override
    {
        assert((*in).size() == _dataCount);
        assert((*in)[0].size() == _backwardOutSize);
        assert(_dataCount > 0);

        for(size_t i = 0; i < _dataCount; ++i)
            for(size_t j = 0; j < _backwardOutSize; ++j)
            {
                /* forwardOut: 順伝播時の出力
                 * in        : ラベルデータ
                 */
                backwardOut[i][j] = (forwardOut[i][j] - (*in)[i][j]) / _dataCount;
            }

        return &backwardOut;
    }
    void init() override {}
    void update() override {}
    void reset() override {}

};

class NetworkModel
{
public:
    NetworkModel(const size_t elemSize, const size_t labelSize)
        : _elemSize(elemSize)
        , _labelSize(labelSize) {}
    ~NetworkModel()
    {
        for(auto& layer : _layers) delete layer;
    }

    void addLayer(Layer *layer)
    {
        if(_layers.size() > 0)
            assert(_layers.back()->forwardOutSize() == layer->backwardOutSize());

        _layers.push_back(layer);
    }
    void addLayer(const Layer::LayerType& type, const size_t numNodes = 0)
    {
        const size_t numPrevNodes = (_layers.size() > 0) ? _layers.back()->forwardOutSize() : _elemSize;

        switch(type)
        {
        case Layer::LayerType::AffineLayer:
            assert(numNodes != 0);
            addLayer(new AffineLayer(numNodes, numPrevNodes)); break;
        case Layer::LayerType::ReLULayer:
            addLayer(new ReLULayer(numPrevNodes)); break;
        case Layer::LayerType::SigmoidLayer:
            addLayer(new SigmoidLayer(numPrevNodes)); break;
        case Layer::LayerType::TanhExpLayer:
            addLayer(new TanhExpLayer(numPrevNodes)); break;
        case Layer::LayerType::DropOutLayer:
            addLayer(new DropOutLayer(numPrevNodes)); break;
        case Layer::LayerType::BatchNormLayer:
            addLayer(new BatchNormLayer(numPrevNodes)); break;
        case Layer::LayerType::SoftmaxLayer:
            addLayer(new SoftMaxLayer(numPrevNodes)); break;
        default:
            return;
        }
    }

    size_t elemSize() const { return _elemSize; }
    size_t labelSize() const { return _labelSize; }
    const std::vector<Layer*>& layers() const { return _layers; }

private:
    const size_t _elemSize;
    const size_t _labelSize;
    std::vector<Layer*> _layers;
};

class LearningModel
{
public:
    LearningModel(const NetworkModel& model) : _networkModel(model) {}

    void setBatchSize(const size_t& batchSize) { _batchSize = batchSize; }
    void setStepCount(const size_t& maxStep) { _stepCount = maxStep; }
    void setTrainData(const vec2d* const x, const vec2d* const t) { train_x = x; train_t = t; }
    void setTestData(const vec2d* const x, const vec2d* const t) { test_x = x; test_t = t; }

    const NetworkModel& networkModel() const { return _networkModel; }
    size_t batchSize() const { return _batchSize; }
    size_t stepCount() const { return _stepCount; }
    const vec2d& get_train_x() const { return *train_x; }
    const vec2d& get_train_t() const { return *train_t; }
    const vec2d& get_test_x() const { return *test_x; }
    const vec2d& get_test_t() const { return *test_t; }

private:
    const NetworkModel& _networkModel;
    size_t _batchSize;
    size_t _stepCount;

    const vec2d* train_x;
    const vec2d* train_t;
    const vec2d* test_x;
    const vec2d* test_t;
};

class Network
{
public:
    Network(const LearningModel& model)
        : model(model) {}

    struct LearningInfo
    {
        size_t step = 0;
        size_t epoch = 0;
        size_t numIter = 0;
        const vec2d* out = nullptr;
        const vec2d* batch_x = nullptr;
        const vec2d* batch_t = nullptr;
        bool breakFlag = false;

        void clear()
        {
            step = 0;
            epoch = 0;
            numIter = 0;
            breakFlag = false;
        }
    } linfo;

    void setObserver(void(*observerFunc)(LearningInfo&, const LearningModel&))
    {
        this->observerFunc = observerFunc;
    }

    void train()
    {
        const std::vector<Layer*>& layers = model.networkModel().layers();
        const vec2d& train_x = model.get_train_x();
        const vec2d& train_t = model.get_train_t();

        const size_t numLayers = layers.size();
        const size_t batchSize = model.batchSize();
        const size_t numIter = train_x.size() / batchSize;
        const size_t stepCount = model.stepCount();

        std::vector<size_t> dataIndexes(train_t.size());
        std::iota(dataIndexes.begin(), dataIndexes.end(), 0);
        vec2d batch_x(batchSize, vec1d(train_x[0].size()));
        vec2d batch_t(batchSize, vec1d(train_t[0].size()));

        assert(batch_x.size() == batch_t.size());
        assert(batch_x[0].size() == layers.front()->backwardOutSize());
        assert(batch_t[0].size() == layers.back()->forwardOutSize());

        PropagationInfo info;
        info.isTraining = true;

        /* 初期化 */
        for(size_t i = 0; i < numLayers; ++i) layers[i]->init();

        linfo.clear();

        linfo.numIter = numIter;

        for(size_t step = 0; step < stepCount; ++step)
        {
            const size_t batchIndex = step % numIter;
            if(batchIndex == 0)
            {
                ++linfo.epoch;
                static std::random_device rnd;
                static std::mt19937 mt(rnd());
                std::shuffle(dataIndexes.begin(), dataIndexes.end(), mt);
            }
            const size_t b = batchIndex * batchSize;
            for(size_t i = 0; i < batchSize; ++i)
            {
                batch_x[i] = train_x[dataIndexes[b + i]];
                batch_t[i] = train_t[dataIndexes[b + i]];
            }

            const vec2d *p = &batch_x;

            /* 順伝播 */
            for(auto& layer : layers) layer->setDataCount(batch_x.size());
            for(auto& layer : layers) p = layer->forward(p, info);

            linfo.out = p;

            p = &batch_t;

            /* 逆伝播 */
            for(size_t i = 0; i < numLayers; ++i)
                p = layers[numLayers - i - 1]->backward(p, info);

            /* 更新 */
            for(auto layer : layers) layer->update();

            /* リセット */
            for(auto layer : layers) layer->reset();


            linfo.step = step;
            linfo.batch_x = &batch_x;
            linfo.batch_t = &batch_t;
            observerFunc(linfo, model);

            if(linfo.breakFlag) break;
        }
    }

    static void chooseBatch(const vec2d& train_x, const vec2d& train_t,
                               vec2d& batch_x, vec2d& batch_t)
    {
        const size_t batchSize = batch_x.size();
        const size_t dataSize = train_x.size();

        assert(batchSize <= dataSize);
        assert(train_x.size() == batch_x.size());

        std::unordered_set<size_t> indexes;
        size_t count = 0;

        static std::random_device rnd;
        static std::mt19937 mt(rnd());

        std::uniform_int_distribution<> randIndex(0, static_cast<int>(dataSize - 1));

        while(count < batchSize)
        {
            const size_t index = randIndex(mt);

            if(indexes.find(index) != indexes.end())
                continue;
            else
            {
                batch_x[count] = train_x[index];
                batch_t[count] = train_t[index];

                indexes.insert(index);

                count++;
            }
        }
    }

    static double loss(const vec2d& batch_x, const vec2d& batch_t)
    {
        assert(batch_x.size() == batch_t.size());
        assert(batch_x[0].size() == batch_t[0].size());

        const size_t dataSize = batch_x.size();
        const size_t labelSize = batch_x[0].size();

        double tmp = 0.0;

        for(size_t i = 0; i < dataSize; ++i)
            for(size_t j = 0; j < labelSize; ++j)
            {
                assert(batch_x[i][j] >= 0);

                tmp += batch_t[i][j] * std::log(batch_x[i][j] + 1e-7);
            }

        return - tmp / dataSize;
    }

    static double accuracy(const NetworkModel& model, const vec2d&x, const vec2d& t)
    {
        const vec2d* acc_x = &x;
        const vec2d* acc_t = &t;

        const vec2d *p = acc_x;

        PropagationInfo info;
        info.isTraining = false;

        const size_t dataCount = acc_t->size();
        const size_t labelCount = (*acc_t)[0].size();

        for(auto& layer : model.layers()) layer->setDataCount(dataCount);
        for(auto& layer : model.layers()) p = layer->forward(p, info);

        assert((*p).size() == acc_t->size());
        assert((*p)[0].size() == (*acc_t)[0].size());
        assert((*acc_x).size() == (*acc_t).size());

        size_t correctCount = 0;

        for(size_t i = 0; i < dataCount; ++i)
        {
            size_t xMaxIndex = 0, tMaxIndex = 0;
            double xMaxValue = - DBL_MAX, tMaxValue = - DBL_MAX;

            for(size_t j = 0; j < labelCount; ++j)
            {
                const double x = (*p)[i][j];
                const double t = (*acc_t)[i][j];

                if(xMaxValue < x)
                {
                    xMaxIndex = j;
                    xMaxValue = x;
                }
                if(tMaxValue < t)
                {
                    tMaxIndex = j;
                    tMaxValue = t;
                }
            }

            if(xMaxIndex == tMaxIndex) correctCount++;
        }

        for(auto layer : model.layers()) layer->reset();

        assert(dataCount > 0);

        return static_cast<double>(correctCount) / static_cast<double>(dataCount);
    }

    static void observer(LearningInfo& info, const LearningModel& model)
    {
        if(info.step % info.numIter != 0) return;

        std::cout << "step:" << info.step << '\t';
        std::cout << "epoch:" << info.epoch << '\t';
        std::cout << "loss:" << Network::loss(*(info.out), *(info.batch_t)) << '\t';
        std::cout << "train-acc:" << Network::accuracy(model.networkModel(), model.get_train_x(), model.get_train_t()) << '\t';
        std::cout << "test-acc:" << Network::accuracy(model.networkModel(), model.get_test_x(), model.get_test_t()) << std::endl;
    }

    static vec2d forward(const NetworkModel& model, const vec2d& input)
    {
        const vec2d* p = &input;
        PropagationInfo info;
        info.isTraining = false;

        for(auto& layer : model.layers()) layer->setDataCount(input.size());
        for(auto& layer : model.layers()) p = layer->forward(p, info);
        for(auto& layer : model.layers()) layer->reset();

        return *p;
    }

private:
    void(*observerFunc)(LearningInfo&, const LearningModel&) = &Network::observer;

    const LearningModel& model;
};






} //namespace nn


#endif // NEURALNETWORK_H
