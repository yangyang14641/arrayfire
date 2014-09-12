#pragma once

#include <stdexcept>
#include <string>
#include <cassert>
#include <af/defines.h>
#include <vector>

class AfError   : public std::logic_error
{
    std::string functionName;
    int lineNumber;
    af_err error;
    AfError();

public:

    AfError(const char * const funcName,
            const int line,
            const char * const message, af_err err);

    AfError(std::string funcName,
            const int line,
            std::string message, af_err err);

	const std::string&
	getFunctionName() const;

    int getLine() const;

    af_err getError() const;

    virtual ~AfError() throw();
};

// TODO: Perhaps add a way to return supported types
class TypeError : public AfError
{
    int argIndex;
    std::string errTypeName;
    TypeError();

public:

    TypeError(const char * const  funcName,
              const int line,
              const int index,
              const af_dtype type);

	const std::string&
	getTypeName() const;

    const int getArgIndex() const;

    ~TypeError() throw() {}
};

class DimensionError : public AfError
{
    int argIndex;
    std::string	expected;
    DimensionError();

public:
    DimensionError(const char * const funcName,
                   const int line,
                   const int index,
                   const char * const expectString);

	const std::string&
	getExpectedCondition() const;

    const int getArgIndex() const;

    ~DimensionError() throw(){}
};

class SupportError  :   public AfError
{
    std::string backend;
    SupportError();
public:
    SupportError(const char * const funcName,
                 const int line,
                 const char * const back);
    ~SupportError()throw() {}
	const std::string&
	getBackendName() const;
};

af_err processException();

#define DIM_ASSERT (INDEX, COND) do {                   \
        if(COND == false) {                             \
            throw DimensionError(__func__, __LINE__,    \
                                 INDEX, #COND);         \
        }                                               \
    } while(0)

#define TYPE_ERROR(INDEX, type) do {            \
        throw TypeError(__func__, __LINE__,     \
                        INDEX, type);           \
    } while(0)                                  \


#define AF_ERROR(MSG, ERR_TYPE) do {            \
        throw AfError(__func__, __LINE__,       \
                      MSG, ERR_TYPE);           \
    } while(0)

#define AF_ASSERT(COND, MESSAGE)                \
    assert(MESSAGE && COND)

#define CATCHALL                                \
    catch(...) {                                \
        return processException();              \
    }
