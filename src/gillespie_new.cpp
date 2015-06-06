
#include <iostream>
#include <random>

#include "GillespieGenerator.h"

/*
 * A Markov model of calcium activated chloride channels
 * which consists of 7 states
 * model contained in the following paper
 * Mechanism of the inhibition of Ca2+ activated Cl- currents by phosphorylation in pulmonary arterial smooth muscle cells
 *
 * 	O_0 [0] <==> O_1 [1] <==> O_2 [2] <==> O_3 [3]
 * 	              |            |            |
 * 	              C_1 [4]     C_2 [5]      C_3 [6]
 */

constexpr const double cai_static = 1.0e-7;

class CaCC_channels
{
	double cai; // calcium [Ca2+]_i [M]
	double v;   // voltage V_m		[mV]

	std::vector<unsigned> initial_state {
					5000, 	// C_0 [50000 ion channels]
					0, 		// C_1
					0,  	// C_2
					0, 		// C_3
					0, 		// O_1
					0, 		// O_2
					0};		// O_3

	auto return_reactions (double calcium_level, double voltage_step) {
		return { gillespie::Reaction(0,1,20000000*calcium_level),
				gillespie::Reaction(1,0,50.0),
				gillespie::Reaction(1,2,20000000*calcium_level),
				gillespie::Reaction(2,1,50.0),
				gillespie::Reaction(2,3,20000000*calcium_level),
				gillespie::Reaction(3,2,50.0),
				gillespie::Reaction(1,4,75),
				gillespie::Reaction(4,1,10./(1+exp((voltage_step-75.0)/-50.0))),
				gillespie::Reaction(2,5,150),
				gillespie::Reaction(5,2,75./(1+exp((voltage_step-120.0)/-50.0))),
				gillespie::Reaction(3,6,300),
				gillespie::Reaction(6,3,100./(1+exp((voltage_step-120.0)/-50.0))
		}
	}

	gillespie::Model cacc_markov;

public:

	CaCC_channels( double calcium , double voltage) :
		cai (calcium),
		v (voltage)
	{}
	CaCC_channels() : CaCC_channels (cai_static, v_static) {}
};

int main()
{

}
