#pragma once
#include <unordered_map>

#include "Types.h"

class Env {
    public:
        Env(Env* outer) : m_outer(outer){}
        Env(Env *outer, ListValue *binds, ListValue *exprs) : m_outer(outer) {
            assert(binds->size() == exprs->size());
            for(size_t i =0; i<binds->size(); i++){
                auto key = binds->at(i)->as_symbol();
                auto val = exprs->at(i);
                set(key, val);
            }
        }
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
