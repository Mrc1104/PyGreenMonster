#include "Amplitude.h"
#include "Waveforms.h"
using namespace BMW;

Ampl::Ampl() : ptr_(std::make_unique<SineAmplitude>()) {}
Ampl::Ampl(Ampl const& other) : ptr_( other.ptr_ ? other.ptr_->Clone() : nullptr ) {}
Ampl& Ampl::operator=(Ampl const& other)
{
	if( this != &other ) {
		ptr_ = other.ptr_ ? other.ptr_->Clone() : nullptr;
	}
	return *this;
}

void Ampl::Set(dataType input) 
{
	ptr_->Set(input);
}
Ampl::dataType Ampl::Get() const
{
	return ptr_->Get();
}

std::unique_ptr<Amplitude> Ampl::Clone() const
{
	return ptr_->Clone();
}

