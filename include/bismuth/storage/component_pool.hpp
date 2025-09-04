#pragma  once
// C++ standard libraries
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include <limits>
#include <cstdint>

namespace bismuth {

class ISparseSet {
    public:
        using EntityID = uint32_t;

        virtual ~ISparseSet() = default;
        virtual void removeComponent(const EntityID& entity) = 0;
};

template<typename ComponentType>
class ComponentPool : public ISparseSet{
    public:
        using EntityID = uint32_t;
        static constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();

        inline ComponentType& getComponent(const EntityID& entity) {
            assert(hasComponent(entity) && "No entity with such component");

            return mDenseComponents[mComponentLocation[entity]];
        }

        inline bool hasComponent(const EntityID& entity) const noexcept {
            return entity < mComponentLocation.size() && 
                   mComponentLocation[entity] != INVALID_INDEX;
        }

        template<typename... Args>
        void addComponent(const EntityID& entity, Args&&... args) {
            if(hasComponent(entity)) {
                mDenseComponents[mComponentLocation[entity]] = ComponentType(std::forward<Args>(args)...);
                return;
            }

            if(entity >= mComponentLocation.size()) {
                mComponentLocation.resize(entity+1, INVALID_INDEX);
            }

            mComponentLocation[entity] = mDenseComponents.size();
            mDenseComponents.push_back(ComponentType(std::forward<Args>(args)...));
            mDenseEntities.push_back(entity);
        }
        void addComponent(const EntityID& entity, ComponentType& component) {
            if(hasComponent(entity)) {
                mDenseComponents[mComponentLocation[entity]] = std::move(component);
                return;
            }

            if(entity >= mComponentLocation.size()) {
                mComponentLocation.resize(entity+1, INVALID_INDEX);
            }

            mComponentLocation[entity] = mDenseComponents.size();
            mDenseComponents.push_back(std::move(component));
            mDenseEntities.push_back(entity);
        }

        void removeComponent(const EntityID& entity) override {
            if(!hasComponent(entity)) {
                return;
            }

            const size_t& index = mComponentLocation[entity];
            const size_t& lastEntity = mDenseEntities.back();

            mDenseComponents[index] = std::move(mDenseComponents.back());
            mDenseEntities[index] = lastEntity;
            mComponentLocation[lastEntity] = index;

            mDenseComponents.pop_back();
            mDenseEntities.pop_back();

            mComponentLocation[entity] = INVALID_INDEX;
        }

        inline void reserve(const size_t& capacity) {
            mComponentLocation.resize(capacity+1, INVALID_INDEX);
            mDenseComponents.reserve(capacity);
            mDenseEntities.reserve(capacity);
        }

        // For efficient reading/sending data to gpu
        const std::vector<ComponentType>& getDenseComponents() const {
            return mDenseComponents;
        }
        const std::vector<uint32_t>& getDenseEntities() const noexcept{
            return mDenseEntities;
        }
        const std::vector<uint32_t>& getComponentLocations() const noexcept {
            return mComponentLocation;
        }

        std::vector<ComponentType>::iterator componentBegin() {
            return mDenseComponents.begin();
        }
        std::vector<ComponentType>::iterator componentEnd() {
            return mDenseComponents.end();
        }

    private:
        std::vector<uint32_t> mComponentLocation;
        std::vector<ComponentType> mDenseComponents;
        std::vector<uint32_t> mDenseEntities;
};

}