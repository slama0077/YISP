#pragma once
#include <unordered_map>

#include "Types.h"

class Env {
    public:
        Env(Env* outer) : m_outer(outer){}
        void set(SymbolValue *key, Value *val){
            m_data[key] = val;
        }
        Env* find(SymbolValue *key) {
            auto search = m_data.find(key);
            if(search != m_data.end()) {
                return this;
            } else if(m_outer){
                return m_outer->find(key);
            }
            return nullptr;
        }
        Value *get(SymbolValue *key){
            auto env = find(key);
            if(!env) {
                throw new ExceptionValue{key->str() + " not found"};
            } else {
                return env->m_data[key];
            }
        }
    private:
        Env *m_outer {nullptr};
        std::unordered_map<SymbolValue *, Value *, HashMapHash, HashMapPred> m_data;
};
