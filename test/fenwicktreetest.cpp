#include "gtest/gtest.h"
#include "qudot/fenwicktree.hpp"

#include "qudot/common.h"
#include <vector>
#include <utility>

TEST(FenwickTreeTest, testTreeFloat) {
    std::vector<std::pair<size_t, float>> worlds;

    worlds.push_back(std::make_pair(2, .25f));
    worlds.push_back(std::make_pair(3, .205f));
    worlds.push_back(std::make_pair(4, .1066f));
    worlds.push_back(std::make_pair(5, .025f));
    worlds.push_back(std::make_pair(6, .0133f));
    worlds.push_back(std::make_pair(7, .0183f));
    worlds.push_back(std::make_pair(8, .00812f));
    worlds.push_back(std::make_pair(9, .00812f));
    worlds.push_back(std::make_pair(10, .0183f));
    worlds.push_back(std::make_pair(11, .0113f));
    worlds.push_back(std::make_pair(12, .025f));
    worlds.push_back(std::make_pair(13, .1066f));
    worlds.push_back(std::make_pair(14, .205f));

    std::vector<float> inputs = {
                0.9511064794259251f,0.38377853329673206f,0.2820770921160032f,0.12061798773512389f,
                0.5233776973122072f,0.10358732167877216f,0.13986631122394888f, .561777f, .5612f, .5867f, .6f,
                .6316f, 0.63963f, 0.66605f, 0.67735f
    };

    qudot::FenwickTree<float> fftree(worlds.size(), qudot::TOLERANCE);
    for (size_t i=0; i < worlds.size(); i++) {
        fftree.update(i, worlds[i]);
    }

    ASSERT_EQ(0, fftree.findIndex(.125f));
    ASSERT_EQ(12, fftree.findIndex(.9999f));
    ASSERT_EQ(1, fftree.findIndex(inputs[1]));
    ASSERT_EQ(2, fftree.findIndex(inputs[4]));
    ASSERT_EQ(0, fftree.findIndex(inputs[5]));
    ASSERT_EQ(3, fftree.findIndex(inputs[7]));
    ASSERT_EQ(2, fftree.findIndex(inputs[8]));
    ASSERT_EQ(4, fftree.findIndex(inputs[9]));
    ASSERT_EQ(5, fftree.findIndex(inputs[10]));
    ASSERT_EQ(7, fftree.findIndex(inputs[11]));
    ASSERT_EQ(8, fftree.findIndex(inputs[12]));
    ASSERT_EQ(10, fftree.findIndex(inputs[13]));
    ASSERT_EQ(10, fftree.findIndex(inputs[14])); 

    ASSERT_EQ(2, fftree.findWorld(.125f).first);
    ASSERT_EQ(14, fftree.findWorld(.9999f).first);
    ASSERT_EQ(3, fftree.findWorld(inputs[1]).first);
    ASSERT_EQ(4, fftree.findWorld(inputs[4]).first);
    ASSERT_EQ(2, fftree.findWorld(inputs[5]).first);
    ASSERT_EQ(5, fftree.findWorld(inputs[7]).first);
    ASSERT_EQ(4, fftree.findWorld(inputs[8]).first);
    ASSERT_EQ(6, fftree.findWorld(inputs[9]).first);
    ASSERT_EQ(7, fftree.findWorld(inputs[10]).first);
    ASSERT_EQ(9, fftree.findWorld(inputs[11]).first);
    ASSERT_EQ(10, fftree.findWorld(inputs[12]).first);
    ASSERT_EQ(12, fftree.findWorld(inputs[13]).first);
    ASSERT_EQ(12, fftree.findWorld(inputs[14]).first);       
}

TEST(FenwickTreeTest, testTreeDouble) {
    std::vector<std::pair<size_t, double>> worlds;

    worlds.push_back(std::make_pair(2, .25d));
    worlds.push_back(std::make_pair(3, .205d));
    worlds.push_back(std::make_pair(4, .1066d));
    worlds.push_back(std::make_pair(5, .025d));
    worlds.push_back(std::make_pair(6, .0133d));
    worlds.push_back(std::make_pair(7, .0183d));
    worlds.push_back(std::make_pair(8, .00812d));
    worlds.push_back(std::make_pair(9, .00812d));
    worlds.push_back(std::make_pair(10, .0183d));
    worlds.push_back(std::make_pair(11, .0113d));
    worlds.push_back(std::make_pair(12, .025d));
    worlds.push_back(std::make_pair(13, .1066d));
    worlds.push_back(std::make_pair(14, .205d));

    std::vector<double> inputs = {
                0.9511064794259251d,0.38377853329673206d,0.2820770921160032d,0.12061798773512389d,
                0.5233776973122072d,0.10358732167877216d,0.13986631122394888d, .561777d, .5612d, .5867d, .6d,
                .6316d, 0.63963d, 0.66605d, 0.67735d
    };

    qudot::FenwickTree<double> fftree(worlds.size(), qudot::TOLERANCE64);
    for (size_t i=0; i < worlds.size(); i++) {
        fftree.update(i, worlds[i]);
    }

    ASSERT_EQ(0, fftree.findIndex(.125d));
    ASSERT_EQ(12, fftree.findIndex(.9999d));
    ASSERT_EQ(1, fftree.findIndex(inputs[1]));
    ASSERT_EQ(2, fftree.findIndex(inputs[4]));
    ASSERT_EQ(0, fftree.findIndex(inputs[5]));
    ASSERT_EQ(3, fftree.findIndex(inputs[7]));
    ASSERT_EQ(2, fftree.findIndex(inputs[8]));
    ASSERT_EQ(4, fftree.findIndex(inputs[9]));
    ASSERT_EQ(5, fftree.findIndex(inputs[10]));
    ASSERT_EQ(7, fftree.findIndex(inputs[11]));
    ASSERT_EQ(8, fftree.findIndex(inputs[12]));
    ASSERT_EQ(10, fftree.findIndex(inputs[13]));
    ASSERT_EQ(10, fftree.findIndex(inputs[14])); 

    ASSERT_EQ(2, fftree.findWorld(.125d).first);
    ASSERT_EQ(14, fftree.findWorld(.9999d).first);
    ASSERT_EQ(3, fftree.findWorld(inputs[1]).first);
    ASSERT_EQ(4, fftree.findWorld(inputs[4]).first);
    ASSERT_EQ(2, fftree.findWorld(inputs[5]).first);
    ASSERT_EQ(5, fftree.findWorld(inputs[7]).first);
    ASSERT_EQ(4, fftree.findWorld(inputs[8]).first);
    ASSERT_EQ(6, fftree.findWorld(inputs[9]).first);
    ASSERT_EQ(7, fftree.findWorld(inputs[10]).first);
    ASSERT_EQ(9, fftree.findWorld(inputs[11]).first);
    ASSERT_EQ(10, fftree.findWorld(inputs[12]).first);
    ASSERT_EQ(12, fftree.findWorld(inputs[13]).first);
    ASSERT_EQ(12, fftree.findWorld(inputs[14]).first);       
}