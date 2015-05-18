//
//  swd.h
//  SimilarImagesSearch
//
//  Created by luba_yaronskaya on 9.05.15.
//  Copyright (c) 2015 LiubovMIPT. All rights reserved.
//

#ifndef SimilarImagesSearch_swd_h
#define SimilarImagesSearch_swd_h

#include <vector>
#include <cmath>

template <typename T>
class SWD
{
    
    typedef std::vector<std::vector<T>> matrix_t;
    
    typedef std::vector<T> vector_t;
    
    matrix_t X;
    
    matrix_t u_f;
    
    matrix_t v_f; //transposed
    
    T rmse; //root mean squared error
    
    T old_rmse;
    
    unsigned features;
    
    T mu;
    
    double eta; //rate of learning
    
    double threshold;
    
    vector_t b_u; //baseline predictors
    
    vector_t b_v;
    
    unsigned long m, n;
    
    const double epsilon = 0.00001;
    
    unsigned total;
    
    unsigned lambda;
    
public:
    
    void run(matrix_t & A, matrix_t &U, matrix_t &V, int f);
    
    double predict(unsigned user, unsigned product);
    
    void print_decomposed();
    
private:
    
    T scal(const vector_t &, const vector_t &);
    
    T values_sum();
    
};

template <typename T>
void SWD<T>::run(matrix_t &R, matrix_t &U, matrix_t &V, int features_)
{
    if (R.empty())
        return;
    
    X = R;
    u_f = U;
    v_f = V;
    
    rmse = 1;
    m = X.size();
    n = X[0].size();
    features = features_;
    b_u = std::vector<T>(m, 0);
    b_v = std::vector<T>(n, 0);
    u_f = std::vector<vector_t>(m, std::vector<T>(features));
    v_f = std::vector<vector_t>(n, std::vector<T>(features));
    for (unsigned u = 0; u < m; ++u)
        for (unsigned f = 0; f < features; ++f)
            u_f[u][f] = 0.1;
    for (unsigned v = 0; v < n; ++v)
        for (unsigned f = 0; f < features; ++f)
            v_f[v][f] = 0.05*f;
    mu = values_sum()/(double)m/n;
    eta = 0.1;
    lambda = 0;
    threshold = 0.1;
    
    while (std::abs(old_rmse - rmse) > epsilon)
    {
        old_rmse = rmse;
        rmse = 0;
        for (unsigned u = 0; u < m; ++u) {
            for (unsigned v = 0; v < n; ++v)
            {
                if (R[u][v] == 0)
                    continue;
                double err = R[u][v] - (mu + b_u[u] + b_v[v] + scal(u_f[u], v_f[v]));
                rmse += err * err;
                mu += eta * err;
                b_u[u] += eta * err;
                b_v[v] += eta * err;
                for (unsigned f = 0; f < features; ++f)
                {
                    u_f[u][f] += eta * (err * v_f[v][f] - lambda * u_f[u][f]);
                    v_f[v][f] += eta * (err * u_f[u][f] - lambda * v_f[v][f]);
                }
            }
        }
        rmse = std::sqrt(rmse / total);
        
        if (rmse > old_rmse - threshold)
        {
            eta *= 0.66;
            threshold *= 0.5;
        }
    }
}

template <typename T>
double SWD<T>::predict(unsigned user, unsigned product)
{
    return mu + b_u[user] + b_v[product] + scal(u_f[user], v_f[product]);
}

template <typename T>
T SWD<T>::scal(const std::vector<T> &x1, const std::vector<T> &x2)
{
    int n = x1.size();
    T res = 0;
    for (int i = 0; i < n; ++i)
        res += x1[i]*x2[i];
    return res;
}

template <typename T>
T SWD<T>::values_sum()
{
    T res = 0;
    for (int u = 0; u < m; ++u)
        for (int v = 0; v < n; ++v)
        {
            T val = X[u][v];
            if (val != 0)
            {
                res += X[u][v];
                total++;
            }
        }
    return res;
    
}

template <typename T>
void SWD<T>::print_decomposed()
{
    for (int u = 0; u < m; ++u) {
        std::cout << "user " << u << " ";
        for (int f = 0; f < features; ++f)
            std::cout << u_f[u][f] << " ";
        std::cout << "\n";
    }
    for (int v = 0; v < n; ++v) {
        std::cout << "product " << v << " ";
        for (int f = 0; f < features; ++f)
            std::cout << v_f[v][f] << " ";
        std::cout << "\n";
    }
    
}

#endif
