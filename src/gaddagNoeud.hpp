#pragma once

#include <map>
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>

class GaddagNode {
public:
    std::unordered_map<char, GaddagNode*> children;
    bool terminal = false;
};