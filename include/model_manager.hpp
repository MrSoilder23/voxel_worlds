#pragma once
// C++ standard libraries
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

// Own libraries
#include "model.hpp"

class ModelManager {
    public:
        static ModelManager& GetInstance();
        ~ModelManager();
        
        void CreateNewModel(const std::string& name, const std::shared_ptr<Model>& model);

        std::shared_ptr<Model> GetModel(const std::string& name);

    private:
        ModelManager();
        ModelManager(ModelManager const&);
        ModelManager operator=(ModelManager const& rhs);

    private:
        std::unordered_map<std::string, std::shared_ptr<Model>> mModels;
};