#include "Waveforms.h"
#include <iostream>

BMW::Cycles::Cycles() = default;
BMW::Cycles::Cycles(std::size_t input)
{
	Set(input);
}
void BMW::Cycles::Set(std::size_t input)
{
	cycles_ = std::clamp(input, lower_bound_, upper_bound_);
}

std::size_t BMW::Cycles::Get() const { return cycles_; }

BMW::Frequency::Frequency() = default;
BMW::Frequency::Frequency(dataType input)
{
	Set(input);
}

void BMW::Frequency::Set(dataType input) {

	value_ = std::clamp(input, lower_bound_, upper_bound_);
}
BMW::Frequency::dataType BMW::Frequency::Get() const { return value_; }


BMW::Ampl::Ampl() : ptr_(std::make_unique<SineAmplitude>()) {}
BMW::Ampl::Ampl(Ampl const& other) : ptr_( other.ptr_ ? other.ptr_->Clone() : nullptr ) {}
BMW::Ampl& BMW::Ampl::operator=(Ampl const& other)
{
	if( this != &other ) {
		ptr_ = other.ptr_ ? other.ptr_->Clone() : nullptr;
	}
	return *this;
}

void BMW::Ampl::Set(dataType input) 
{
	ptr_->Set(input);
}
BMW::Ampl::dataType BMW::Ampl::Get() const
{
	return ptr_->Get();
}

std::unique_ptr<BMW::Amplitude> BMW::Ampl::Clone() const
{
	return ptr_->Clone();
}
