#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::ValidityCheck(Position& pos) const{
    if (!pos.IsValid()){
        throw InvalidPositionException("Invalid cell position");
    }
}

void Sheet::SetCell(Position pos, std::string text) {
        ValidityCheck(pos);
        cells_.resize(std::max(pos.row + 1, int(std::size(cells_))));
        cells_[pos.row].resize(std::max(pos.col+1, int(cells_[pos.row].size())));
        if (!cells_[pos.row][pos.col]){
            cells_[pos.row][pos.col] = std::make_unique<Cell>(*this);
        }
        
        cells_[pos.row][pos.col]->Set(std::move(text));
}

const CellInterface* Sheet::GetCell(Position pos) const {
    ValidityCheck(pos);
        if (pos.row < int(std::size(cells_)) && pos.col < int(std::size(cells_[pos.row]))) {
            if (cells_[pos.row][pos.col].get()->GetText() == "") {
                return nullptr;
        }
            else return cells_[pos.row][pos.col].get();  
    }
        else return nullptr;
}
CellInterface* Sheet::GetCell(Position pos) {
ValidityCheck(pos);
        if (pos.row < int(std::size(cells_)) && pos.col < int(std::size(cells_[pos.row]))) {
            if (cells_[pos.row][pos.col].get()->GetText() == "") {
                return nullptr;
        }
            else return cells_[pos.row][pos.col].get();  
    }
        else return nullptr;
}

Cell* Sheet::Get_Cell(Position pos){
    ValidityCheck(pos);
        if (pos.row < int(std::size(cells_)) && pos.col < int(std::size(cells_[pos.row]))) {
            return cells_[pos.row][pos.col].get();   
        } else {
            return nullptr;
        }   
}

const Cell* Sheet::Get_Cell(Position pos) const {
    const Cell* const_cell = Get_Cell(pos);
    return const_cell;
}

void Sheet::ClearCell(Position pos) {
     ValidityCheck(pos);
        if (pos.row < int(std::size(cells_)) && pos.col < int(std::size(cells_[pos.row]))) {
            if (cells_[pos.row][pos.col]) {
                cells_[pos.row][pos.col]->Clear();
                
                 if (!cells_[pos.row][pos.col]->IsReferenced()) {
                    cells_[pos.row][pos.col].reset();
                }
        }
    }
}

Size Sheet::GetPrintableSize() const {
     Size size;
    
    for (int row = 0; row < int(std::size(cells_)); ++row) {       
        for (int col = (int(std::size(cells_[row])) - 1); col >= 0; --col) {
  
            if (cells_[row][col]) {
                
                if (cells_[row][col]->GetText().empty()) {
                    continue;
                    
                } else {
                    size.rows = std::max(size.rows, row + 1);
                    size.cols = std::max(size.cols, col + 1);
                    break;
                }
            }
        }
    }  
    return size;
}

void Sheet::PrintValues(std::ostream& output) const {
    Size sheet_size = GetPrintableSize();
    for (int row = 0; row < sheet_size.rows; ++row){
         for (int col = 0; col < sheet_size.cols; ++col){
             if (col > 0) output<<'\t';
             if (col < int(std::size(cells_[row]))){
                 if (cells_[row][col]) {
                     std::visit([&output](const auto& obj){output<<obj;},cells_[row][col]->GetValue());
                 }
             }
         }
        output<<'\n';
    }
}
void Sheet::PrintTexts(std::ostream& output) const {
    Size sheet_size = GetPrintableSize();
     for (int row = 0; row < sheet_size.rows; ++row) {        
        for (int col = 0; col < sheet_size.cols; ++col) {
            
            if (col) {output << '\t';}
            
            if (col < int(std::size(cells_[row]))) {                
                if (cells_[row][col]) {output << cells_[row][col]->GetText();}
            }
        }
        
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}