#pragma once
#include <vector>
#include <cassert>
#include "NonogramCell.h"

using std::vector;

class NonogramField {
private:
	// 1 - filled, 0 - empty, -1 - undefined
	vector< vector<NonogramCell> > field_;
	// vector< vector<NonogramCell> > previousField_;

	int columnCount_, rowCount_;

	template <typename CellType, typename FieldPointer>
	class basic_column_iterator : public std::iterator<std::random_access_iterator_tag, int> {
	private:
		FieldPointer field_;
		int column_;
		int index_;

		basic_column_iterator(FieldPointer field, int column, int index) {
			field_ = field;
			index_ = index;
			column_ = column;
		}

	public:
		basic_column_iterator() {};

		template <typename CellType, typename FieldPointer>
		basic_column_iterator(const basic_column_iterator<CellType, FieldPointer> &other) {
			this->index_ = other.index_;
			this->column_ = other.column_;
		}

		template <typename CellType, typename FieldPointer>
		bool operator <(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return this->index < other.index;
		}

		template <typename CellType>
		bool operator >(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return (other < (*this));
		}

		template <typename CellType, typename FieldPointer>
		bool operator ==(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return (!((*this) < other) && !(other < (*this)));
		}

		template <typename CellType, typename FieldPointer>
		bool operator !=(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return (!((*this) == other));
		}

		template <typename CellType, typename FieldPointer>
		bool operator <=(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return (!((*this) > other));
		}

		template <typename CellType, typename FieldPointer>
		bool operator >=(const basic_column_iterator<CellType, FieldPointer> &other) const {
			return (!((*this) < other));
		}

		basic_column_iterator& operator ++() {
			++index;
			return (*this);
		}

		basic_column_iterator operator ++(int) {
			basic_column_iterator ans = *this;
			++index;
			return ans;
		}

		basic_column_iterator& operator --() {
			assert(index > 0);
			--index;
			return (*this);
		}

		basic_column_iterator operator --(int) {
			assert(index_ > 0);
			basic_column_iterator ans = *this;
			--index_;
			return ans;
		}

		basic_column_iterator& operator +=(ptrdiff_t shift) {
			assert(index_ + shift >= 0);
			index_ += shift;
			return (*this);
		}

		basic_column_iterator operator +(ptrdiff_t shift) const {
			basic_column_iterator ans = *this;
			ans += shift;
			return ans;
		}

		friend basic_column_iterator operator +(ptrdiff_t shift, const basic_column_iterator &other) {
			return other + shift;
		}

		basic_column_iterator& operator -=(ptrdiff_t shift) {
			assert((ptrdiff_t)index - shift >= 0);
			index -= shift;
			return (*this);
		}

		basic_column_iterator operator -(ptrdiff_t shift) const {
			basic_iterator ans = *this;
			ans -= shift;
			return ans;
		}

		ptrdiff_t operator -(const basic_column_iterator &other) const {
			return this->index - other.index;
		}

		CellType& operator *() const {
			assert(index_ < field_->size());
			return field_->at(index_)[column_];
		}

		CellType& operator [](ptrdiff_t shift) const {
			return *(*this + shift);
		}

		friend class NonogramField;

		friend class basic_column_iterator<CellType, FieldPointer>;
	};

public:
	NonogramField(int columnCount, int rowCount);

	bool HasUndefinedCells() const;

	typedef basic_column_iterator<const NonogramCell, const vector< vector<NonogramCell> >*> const_column_iterator;
	typedef basic_column_iterator<NonogramCell, vector< vector<NonogramCell> >*> column_iterator;
	typedef vector<NonogramCell>::const_iterator const_row_iterator;
	typedef vector<NonogramCell>::iterator row_iterator;

	const_column_iterator ColumnCBegin(int column) const;
	column_iterator ColumnBegin(int column);
	const_column_iterator ColumnCEnd(int column) const;
	column_iterator ColumnEnd(int column);

	const_row_iterator RowBegin(int row) const;
	row_iterator RowBegin(int row);
	const_row_iterator RowEnd(int row) const;
	row_iterator RowEnd(int row);

	NonogramCell GetCell(int row, int column) const;
	void SetCell(int row, int column, NonogramCell value);

	void SetFieldSize(int rowCount, int columnColunt);

	int GetColumnCount() const;
	int GetRowCount() const;

	void Print() const;
};
