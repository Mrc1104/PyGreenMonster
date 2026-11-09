#include "Waveforms.h"
#include <iostream>

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
