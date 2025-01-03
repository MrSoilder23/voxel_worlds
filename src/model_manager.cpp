// Our libraries
#include "model_manager.hpp"

ModelManager::ModelManager() {

}
ModelManager::ModelManager(ModelManager const&) {

}
ModelManager ModelManager::operator=(ModelManager const& rhs) {
    return *this;
}

ModelManager& ModelManager::GetInstance() {
    static ModelManager sInstance;
    return sInstance;
}
ModelManager::~ModelManager() {
    std::cout << "ModelManager bye bye" << std::endl;
}

void ModelManager::CreateNewModel(const std::string& name, const Model& model) {
    mModels[name] = model;
}

Model ModelManager::GetModel(const std::string& name) {
    auto search = mModels.find(name);
    if(search != mModels.end()) {
        return search->second;
    } else {
        std::cerr << "Could not find the model: " << name << std::endl;
        return Model();
    }
}
