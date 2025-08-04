#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iomanip>

/*
 * TP7 - EXCEPTIONS
 */

class DataFrameException : public std::exception {
protected:
    std::string m_message;
public:
    DataFrameException(const std::string& message) : m_message(message) {}
    virtual const char* what() const noexcept override { return m_message.c_str(); }
};

class ColumnNotFoundException : public DataFrameException {
public:
    ColumnNotFoundException(const std::string& columnName)
        : DataFrameException("Column not found: " + columnName) {}
};

class IndexOutOfBoundsException : public DataFrameException {
public:
    IndexOutOfBoundsException(size_t index, size_t limit)
        : DataFrameException("Index out of bounds: " + std::to_string(index) + " (limit: " + std::to_string(limit) + ")") {}
};

class TypeMismatchException : public DataFrameException {
public:
    TypeMismatchException(const std::string& expected, const std::string& actual)
        : DataFrameException("Type mismatch: expected " + expected + ", got " + actual) {}
};

class Value {
public:
    static const int TYPE_NONE = 0;
    static const int TYPE_INTEGER = 1;
    static const int TYPE_DOUBLE = 2;
    static const int TYPE_STRING = 3;

private:
    int m_type;
    int m_intValue;
    double m_doubleValue;
    std::string m_stringValue;

public:
    Value() : m_type(TYPE_NONE), m_intValue(0), m_doubleValue(0.0) {}
    Value(int value) : m_type(TYPE_INTEGER), m_intValue(value), m_doubleValue(0.0) {}
    Value(double value) : m_type(TYPE_DOUBLE), m_intValue(0), m_doubleValue(value) {}
    Value(const std::string& value) : m_type(TYPE_STRING), m_intValue(0), m_doubleValue(0.0), m_stringValue(value) {}
    Value(const char* value) : m_type(TYPE_STRING), m_intValue(0), m_doubleValue(0.0), m_stringValue(value) {}

    int asInt() const {
        if (m_type != TYPE_INTEGER) {
            throw TypeMismatchException("INTEGER", getTypeName(m_type));
        }
        return m_intValue;
    }

    double asDouble() const {
        if (m_type == TYPE_DOUBLE) return m_doubleValue;
        if (m_type == TYPE_INTEGER) return static_cast<double>(m_intValue);
        throw TypeMismatchException("numeric", getTypeName(m_type));
    }

    std::string asString() const {
        if (m_type == TYPE_STRING) return m_stringValue;
        if (m_type == TYPE_INTEGER) return std::to_string(m_intValue);
        if (m_type == TYPE_DOUBLE) return std::to_string(m_doubleValue);
        return "NA";
    }

    int getType() const { return m_type; }
    bool isNumeric() const { return m_type == TYPE_INTEGER || m_type == TYPE_DOUBLE; }
    bool isNone() const { return m_type == TYPE_NONE; }

    std::string toString() const {
        switch (m_type) {
            case TYPE_INTEGER: return std::to_string(m_intValue);
            case TYPE_DOUBLE: return std::to_string(m_doubleValue);
            case TYPE_STRING: return m_stringValue;
            case TYPE_NONE: return "NA";
            default: return "UNKNOWN";
        }
    }

    static std::string getTypeName(int type) {
        switch (type) {
            case TYPE_INTEGER: return "INTEGER";
            case TYPE_DOUBLE: return "DOUBLE";
            case TYPE_STRING: return "STRING";
            case TYPE_NONE: return "NONE";
            default: return "UNKNOWN";
        }
    }
};

class Column {
private:
    std::string m_name;
    int m_type;
    std::vector<Value> m_values;

public:
    Column(const std::string& name, int type);
    const std::string& getName();
    int getType();
    size_t size();
    Value& at(size_t index);
    void addValue(const Value& value);
    bool isNumeric() const;
    double mean();
};

Column::Column(const std::string& name, int type) : m_name(name), m_type(type) {
    if (type != Value::TYPE_INTEGER && type != Value::TYPE_DOUBLE && type != Value::TYPE_STRING) {
        throw std::invalid_argument("Invalid column type");
    }
}

const std::string& Column::getName() { return m_name; }
int Column::getType() { return m_type; }
size_t Column::size() { return m_values.size(); }

Value& Column::at(size_t index) {
    if (index >= m_values.size()) {
        throw IndexOutOfBoundsException(index, m_values.size());
    }
    return m_values[index];
}

void Column::addValue(const Value& value) {
    if (value.getType() != m_type) {
        throw TypeMismatchException(Value::getTypeName(m_type), Value::getTypeName(value.getType()));
    }
    m_values.push_back(value);
}

bool Column::isNumeric() const {
    return m_type == Value::TYPE_INTEGER || m_type == Value::TYPE_DOUBLE;
}

double Column::mean() {
    if (!isNumeric()) {
        throw TypeMismatchException("numeric", Value::getTypeName(m_type));
    }
    if (m_values.empty()) {
        throw DataFrameException("Cannot compute mean of empty column");
    }
    double sum = 0;
    for (const auto& v : m_values) sum += v.asDouble();
    return sum / m_values.size();
}

class DataFrame {
private:
    std::vector<std::string> m_columnNames;
    std::map<std::string, std::unique_ptr<Column>> m_columns;
    size_t m_rowCount;

public:
    DataFrame();
    void addColumn(const std::string& name, int type);
    bool hasColumn(const std::string& name);
    Column& getColumn(const std::string& name);
    size_t rowCount() const;
    size_t columnCount() const;
    const std::vector<std::string>& getColumnNames();
    void addRow(const std::map<std::string, Value>& values);
    Value getValue(const std::string& columnName, size_t rowIndex);
    void setValue(const std::string& columnName, size_t rowIndex, const Value& value);
    void print(std::ostream& os = std::cout, size_t maxRows = 10);
};

DataFrame::DataFrame() : m_rowCount(0) {}

void DataFrame::addColumn(const std::string& name, int type) {
    if (m_columns.find(name) != m_columns.end()) {
        throw DataFrameException("Column '" + name + "' already exists.");
    }
    m_columns[name] = std::make_unique<Column>(name, type);
    m_columnNames.push_back(name);
    for (size_t i = 0; i < m_rowCount; ++i) {
        m_columns[name]->addValue(Value());
    }
}

bool DataFrame::hasColumn(const std::string& name) {
    return m_columns.find(name) != m_columns.end();
}

Column& DataFrame::getColumn(const std::string& name) {
    if (!hasColumn(name)) {
        throw ColumnNotFoundException(name);
    }
    return *m_columns[name];
}

size_t DataFrame::rowCount() const { return m_rowCount; }
size_t DataFrame::columnCount() const { return m_columnNames.size(); }

const std::vector<std::string>& DataFrame::getColumnNames() {
    return m_columnNames;
}

void DataFrame::addRow(const std::map<std::string, Value>& values) {
    for (const auto& pair : values) {
        const std::string& columnName = pair.first;
        const Value& value = pair.second;
        if (!hasColumn(columnName)) {
            throw ColumnNotFoundException(columnName);
        }
        Column& column = getColumn(columnName);
        if (value.getType() != column.getType()) {
            throw TypeMismatchException(Value::getTypeName(column.getType()), Value::getTypeName(value.getType()));
        }
        column.addValue(value);
    }
    ++m_rowCount;
    for (const auto& name : m_columnNames) {
        if (values.find(name) == values.end()) {
            m_columns[name]->addValue(Value());
        }
    }
}

Value DataFrame::getValue(const std::string& columnName, size_t rowIndex) {
    if (!hasColumn(columnName)) {
        throw ColumnNotFoundException(columnName);
    }
    Column& column = getColumn(columnName);
    if (rowIndex >= rowCount()) {
        throw IndexOutOfBoundsException(rowIndex, rowCount());
    }
    return column.at(rowIndex);
}

void DataFrame::setValue(const std::string& columnName, size_t rowIndex, const Value& value) {
    if (!hasColumn(columnName)) {
        throw ColumnNotFoundException(columnName);
    }
    Column& column = getColumn(columnName);
    if (rowIndex >= rowCount()) {
        throw IndexOutOfBoundsException(rowIndex, rowCount());
    }
    if (value.getType() != column.getType()) {
        throw TypeMismatchException(Value::getTypeName(column.getType()), Value::getTypeName(value.getType()));
    }
    column.at(rowIndex) = value;
}

void DataFrame::print(std::ostream& os, size_t maxRows) {
    for (const auto& name : m_columnNames) {
        os << name << "\t";
    }
    os << "\n";

    for (size_t i = 0; i < rowCount() && i < maxRows; ++i) {
        for (const auto& name : m_columnNames) {
            os << getValue(name, i).toString() << "\t";
        }
        os << "\n";
    }

    if (rowCount() > maxRows) {
        os << "... (truncated)\n";
    }
}


int main() {
    try {
        DataFrame df;

        // Add columns
        df.addColumn("Name", Value::TYPE_STRING);
        df.addColumn("Age", Value::TYPE_INTEGER);
        df.addColumn("Score", Value::TYPE_DOUBLE);

        // Add rows
        df.addRow({
            {"Name", "Aymane"},
            {"Age", 23},
            {"Score", 91.5}
        });

        df.addRow({
            {"Name", "Victor"},
            {"Age", 27},
            {"Score", 88.0}
        });

        df.addRow({
            {"Name", "Yedam"},
            {"Age", 25},
            {"Score", 93.2}
        });

        // Print DataFrame
        std::cout << "=== DataFrame ===" << std::endl;
        df.print();

        // Test getValue
        std::cout << "Name at row 1: " << df.getValue("Name", 1).asString() << std::endl;

        // Test setValue
        df.setValue("Score", 0, Value(99.9));
        std::cout << "Updated Score at row 0: " << df.getValue("Score", 0).asDouble() << std::endl;

        // Test mean()
        std::cout << "Mean Age: " << df.getColumn("Age").mean() << std::endl;
        std::cout << "Mean Score: " << df.getColumn("Score").mean() << std::endl;

        // Exception: access non-existing column
        try {
            df.getValue("Height", 0);
        } catch (const ColumnNotFoundException& e) {
            std::cout << "[Exception] " << e.what() << std::endl;
        }

        // Exception: access out of bounds
        try {
            df.getValue("Name", 10);
        } catch (const IndexOutOfBoundsException& e) {
            std::cout << "[Exception] " << e.what() << std::endl;
        }

        // Exception: type mismatch
        try {
            int wrongType = df.getValue("Name", 0).asInt();
        } catch (const TypeMismatchException& e) {
            std::cout << "[Exception] " << e.what() << std::endl;
        }

        // Exception: mean() on non-numeric column
        try {
            double m = df.getColumn("Name").mean();
        } catch (const TypeMismatchException& e) {
            std::cout << "[Exception] " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
