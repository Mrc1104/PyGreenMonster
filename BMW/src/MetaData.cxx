#include "MetaData.h"
using namespace BMW;
const std::array<Meta, MetaData::N_CHANNELS> MetaData::meta_ = {
	Meta{Name("MHF1C08H"), ID( 0), TrimCard::BMOD1, TrimChan::CHAN0, Hall::A, Modtype::Coil  , Descr("1C08 Horizontal Coil")},
	Meta{Name("MHF1C08V"), ID( 1), TrimCard::BMOD1, TrimChan::CHAN1, Hall::A, Modtype::Coil  , Descr("1C08 Verical Coil")   },
	Meta{Name("MHF1C10H"), ID( 2), TrimCard::BMOD1, TrimChan::CHAN2, Hall::A, Modtype::Coil  , Descr("1C10 Horizontal Coil")},
	Meta{Name("MHF1C10V"), ID( 3), TrimCard::BMOD1, TrimChan::CHAN3, Hall::A, Modtype::Coil  , Descr("1C10 Verical Coil")   },
	Meta{Name("Ramp")    , ID( 4), TrimCard::BMOD2, TrimChan::CHAN0, Hall::A, Modtype::Coil  , Descr("Hall A Ramp")         },
	Meta{Name("MHF1C02V"), ID( 5), TrimCard::BMOD2, TrimChan::CHAN1, Hall::A, Modtype::Coil  , Descr("1C02 Horizontal Coil")},
	Meta{Name("MHF1C03H"), ID( 6), TrimCard::BMOD2, TrimChan::CHAN2, Hall::A, Modtype::Coil  , Descr("1C03 Verical Coil")   },
	Meta{Name("SL20EMod"), ID( 7), TrimCard::BMOD2, TrimChan::CHAN3, Hall::A, Modtype::Energy, Descr("Energy Modulation")   },

	Meta{Name("MHF1C08H"), ID( 8), TrimCard::BMOD1, TrimChan::CHAN0, Hall::C, Modtype::Coil  , Descr("1C08 Horizontal Coil")},
	Meta{Name("MHF1C08V"), ID( 9), TrimCard::BMOD1, TrimChan::CHAN1, Hall::C, Modtype::Coil  , Descr("1C08 Verical Coil")   },
	Meta{Name("MHF1C10H"), ID(10), TrimCard::BMOD1, TrimChan::CHAN2, Hall::C, Modtype::Coil  , Descr("1C10 Horizontal Coil")},
	Meta{Name("MHF1C10V"), ID(11), TrimCard::BMOD1, TrimChan::CHAN3, Hall::C, Modtype::Coil  , Descr("1C10 Verical Coil")   },
	Meta{Name("Ramp")    , ID(12), TrimCard::BMOD2, TrimChan::CHAN0, Hall::C, Modtype::Coil  , Descr("Hall C Ramp")         },
	Meta{Name("EMPTY")   , ID(13), TrimCard::BMOD2, TrimChan::CHAN1, Hall::C, Modtype::Empty , Descr("Empty")               },
	Meta{Name("EMPTY")   , ID(14), TrimCard::BMOD2, TrimChan::CHAN2, Hall::C, Modtype::Empty , Descr("Empty")               },
	Meta{Name("SL20EMod"), ID(15), TrimCard::BMOD2, TrimChan::CHAN3, Hall::C, Modtype::Energy, Descr("Energy Modulation")   }
};

std::string Meta::GetPV() const
{
	std::string pv = stringify(card_) + ':' + stringify(chan_);
	return pv;
}
std::string BMW::stringify(BMW::TrimCard card) { return (card == TrimCard::BMOD1) ? "BMOD1" : "BMOD2"; }
std::string BMW::stringify(BMW::TrimChan chan) { 
	std::string str;
	switch(chan) {
		case BMW::TrimChan::CHAN0: str = "CHAN0"; break;
		case BMW::TrimChan::CHAN1: str = "CHAN1"; break;
		case BMW::TrimChan::CHAN2: str = "CHAN2"; break;
		case BMW::TrimChan::CHAN3: str = "CHAN3"; break;
	};
	return str;
};

MetaData& MetaData::Instance()
{
	static MetaData instance;
	return instance;
}

