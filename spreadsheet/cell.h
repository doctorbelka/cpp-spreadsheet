#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
    Cell();
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

private:
    class Impl {
        public:
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const;
        virtual bool HasCache();
        virtual void InvalidateCache();
        virtual ~Impl() = default;
    };
    
    class EmptyImpl : public Impl{
        public:
        Value GetValue() const override;
        std::string GetText() const override;
    };
    
    class TextImpl : public Impl{
        public:
        explicit TextImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;
        private:
        std::string text_;
    };
    
    class FormulaImpl : public Impl{
        public:
        explicit FormulaImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;
        virtual bool HasCache();
        virtual void InvalidateCache();
        private:
        mutable std::optional<FormulaInterface::Value> cache_;
        std::unique_ptr<FormulaInterface> formula_;
    };

    SheetInterface& sheet_;
    std::unique_ptr<Impl> impl_;
    std::set<Cell*> dependent_cells_;   
    std::set<Cell*> referenced_cells_; 

};