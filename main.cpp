#include "dbscan.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

struct TrackObj {
    int frame_idx;
    int obj_id;
    double x, y, width, height, yaw;
};

NDArray<double, 2> readFramesData(const std::string &filename, size_t numPoints, size_t dimensions)
{
    std::ifstream infile(filename); 
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    std::string line; 

    NDArray<double, 2> data({numPoints, dimensions});
    size_t pointIndex = 0;

    while (std::getline(infile, line)) { 
        std::istringstream iss(line);    
        TrackObj obj;

        char delimiter;                       // skip comma
        if (iss >> obj.frame_idx >> delimiter // read frame_idx
            >> obj.obj_id >> delimiter        // read obj_id
            >> obj.x >> delimiter             // read x
            >> obj.y >> delimiter             // read y
            >> obj.width >> delimiter         // read width
            >> obj.height >> delimiter        // read height
            >> obj.yaw) {                     // read yaw

            //normalize
            data[pointIndex][0] = obj.x / 4096;
            data[pointIndex][1] = (obj.y + obj.height) / 1800;
            data[pointIndex][2] = obj.yaw / 2 / 3.1415926;
            pointIndex++;
            data[pointIndex][0] = (obj.x + obj.width) / 4096;
            data[pointIndex][1] = (obj.y + obj.height) / 1800;
            data[pointIndex][2] = obj.yaw / 2 / 3.1415926;
            pointIndex++;
            data[pointIndex][0] = (obj.x + obj.width / 2) / 4096;
            data[pointIndex][1] = (obj.y + obj.height) / 1800;
            data[pointIndex][2] = obj.yaw / 2 / 3.1415926;
            pointIndex++;
        }
        else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    infile.close(); 
    return data;
}

void saveClusterResults(const NDArray<double, 2> &points, const std::vector<std::atomic_int> &labels, const std::string &filename)
{
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    for (size_t i = 0; i < points.dim(0); ++i) {
        outfile << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << labels[i] << "\n";
    }

    outfile.close();
}

int main(void)
{
    size_t points_num = 11157;
    NDArray<double, 2> points({points_num * 3, 3});
    points = readFramesData("../data/frames_data_full.txt", points_num * 3, 3);

    size_t numPoints = points.dim(0);
    size_t dimensions = 3;
    double eps = 0.05;
    size_t minPts = 15;

    std::cout << "Number of Points        : " << numPoints << std::endl;
    std::cout << "Dimensions              : " << dimensions << std::endl;
    std::cout << "Epsilon (eps)           : " << eps << std::endl;
    std::cout << "Minimum Points (minPts) : " << minPts << std::endl;

    std::cout << "Start!" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    DBSCAN<double> dbscan(points, eps, minPts, std::thread::hardware_concurrency());
    dbscan.run();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

    const std::vector<std::atomic_int> &labels = dbscan.labels();
    std::cout << "labels size: " << labels.size() << std::endl;
    std::cout << "nClusters: " << dbscan.nClusters() << std::endl;

    saveClusterResults(points, labels, "../data/frames_data_full_cluster.txt");

    return 0;
}