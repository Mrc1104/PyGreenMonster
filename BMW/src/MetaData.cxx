#include "MetaData.h"
using namespace BMW;
const std::array<Meta, MetaData::N_CHANNELS> MetaData::meta_ = {
	Meta{Name("MHF1C08H"sv), ID( 0), TrimCard::BMOD1, TrimChan::CHAN0, Hall::A, Modtype::Coil  , Descr("1C08 Horizontal Coil"sv)},
	Meta{Name("MHF1C08V"sv), ID( 1), TrimCard::BMOD1, TrimChan::CHAN1, Hall::A, Modtype::Coil  , Descr("1C08 Verical Coil"sv)   },
	Meta{Name("MHF1C10H"sv), ID( 2), TrimCard::BMOD1, TrimChan::CHAN2, Hall::A, Modtype::Coil  , Descr("1C10 Horizontal Coil"sv)},
	Meta{Name("MHF1C10V"sv), ID( 3), TrimCard::BMOD1, TrimChan::CHAN3, Hall::A, Modtype::Coil  , Descr("1C10 Verical Coil"sv)   },
	Meta{Name("Ramp"sv)    , ID( 4), TrimCard::BMOD2, TrimChan::CHAN0, Hall::A, Modtype::Coil  , Descr("Hall A Ramp"sv)         },
	Meta{Name("MHF1C02V"sv), ID( 5), TrimCard::BMOD2, TrimChan::CHAN1, Hall::A, Modtype::Coil  , Descr("1C02 Horizontal Coil"sv)},
	Meta{Name("MHF1C03H"sv), ID( 6), TrimCard::BMOD2, TrimChan::CHAN2, Hall::A, Modtype::Coil  , Descr("1C03 Verical Coil"sv)   },
	Meta{Name("SL20EMod"sv), ID( 7), TrimCard::BMOD2, TrimChan::CHAN3, Hall::A, Modtype::Energy, Descr("Energy Modulation"sv)   },

	Meta{Name("MHF1C08H"sv), ID( 8), TrimCard::BMOD1, TrimChan::CHAN0, Hall::C, Modtype::Coil  , Descr("1C08 Horizontal Coil"sv)},
	Meta{Name("MHF1C08V"sv), ID( 9), TrimCard::BMOD1, TrimChan::CHAN1, Hall::C, Modtype::Coil  , Descr("1C08 Verical Coil"sv)   },
	Meta{Name("MHF1C10H"sv), ID(10), TrimCard::BMOD1, TrimChan::CHAN2, Hall::C, Modtype::Coil  , Descr("1C10 Horizontal Coil"sv)},
	Meta{Name("MHF1C10V"sv), ID(11), TrimCard::BMOD1, TrimChan::CHAN3, Hall::C, Modtype::Coil  , Descr("1C10 Verical Coil"sv)   },
	Meta{Name("Ramp"sv)    , ID(12), TrimCard::BMOD2, TrimChan::CHAN0, Hall::C, Modtype::Coil  , Descr("Hall C Ramp"sv)         },
	Meta{Name("EMPTY"sv)   , ID(13), TrimCard::BMOD2, TrimChan::CHAN1, Hall::C, Modtype::Empty , Descr("Empty"sv)               },
	Meta{Name("EMPTY"sv)   , ID(14), TrimCard::BMOD2, TrimChan::CHAN2, Hall::C, Modtype::Empty , Descr("Empty"sv)               },
	Meta{Name("SL20EMod"sv), ID(15), TrimCard::BMOD2, TrimChan::CHAN3, Hall::C, Modtype::Energy, Descr("Energy Modulation"sv)   }
};

