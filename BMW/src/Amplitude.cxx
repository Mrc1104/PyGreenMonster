#include "Amplitude.h"
#include "Waveforms.h"
using namespace BMW;

Ampl::Ampl() : ptr_(std::make_unique<SineAmplitude>()) {}
Ampl::Ampl(dataType input) : Ampl()
{
	Set(input);
}

Ampl::Ampl(std::string_view pv, dataType input) : Ampl(input)
{
	Attach(pv);
}
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

void Ampl::Attach(std::string_view pv)
{
	ptr_->Attach(pv);
}

bool Ampl::Broadcast()
{
	return ptr_->Broadcast();
}
bool Ampl::SetAndBroadcast(dataType input)
{
	ptr_->Set(input);
	return ptr_->Broadcast();
}

std::optional<Ampl::dataType> Ampl::GetRemote() const
{
	return ptr_->GetRemote();	
}
