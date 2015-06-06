/*
 * GillespieGenerator.h
 *
 *  Created on: 5 Jun 2015
 *      Author: haroon
 *
 *  Common problems:
 *  	* uniform_random_number has issue with std::random_device due to issues with the hardware
 *  		random generator in GCC compiler
 *  	  uniform_random_number has (current) issues with std::chrono in visual C++
 *  	  	compiler (if C++11 compliant
 *  	  these are taken into account with macros
 *
 */



#ifndef GILLESPIEGENERATOR_H_
#define GILLESPIEGENERATOR_H_

#include <random>
#include <chrono>
#include <iostream>
#include <array>
#include <vector>
#include <initializer_list>

namespace gillespie {

	/*
	 * random uniform generator
	 */
	template <class Engine = std::mt19937>
	class uniform_random_number{
	private:
		Engine gen;
		std::random_device rd;
		std::uniform_real_distribution<> dis;
	public:

		uniform_random_number() :
			uniform_random_number (0,1)
		{};

		explicit uniform_random_number(double b) :
			uniform_random_number (0,b)
		{}

		uniform_random_number(double f, double b) :
	#ifdef __GNUC__
			gen(std::chrono::system_clock::now().time_since_epoch().count()),
	#endif
	#ifdef _MSC_VER
			gen(rd),
	#endif
			dis(f,b)
		{
			std::cout << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
		}

		inline auto operator() () noexcept {
			return dis(gen);
		}
	};


	/*
	 * Reaction(s_from, s_to, transition_rate)
	 * s_from - state the reaction is from (sink)
	 * s_fo - reaction flux to source
	 * transition_rate - the rate the trantion between s_from and s_to
	 */
	class Reaction {
	public:
		constexpr Reaction(const unsigned sfrom_, const unsigned sto_, double rate_) noexcept
			: s_from (sfrom_), s_to(sto_), transition_rate(rate_)
		{}

		//Reaction (const Reaction&) = delete; // each Reaction is unique, prevent copies

		Reaction (Reaction&& rea) = default; // the types are known => synthesise default compiler version
		/*
		Reaction (Reaction&& rea) :
			s_from( std::move(rea.s_from) ),
			s_to( std::move(rea.s_to) )
		{
			rea.s_from = 0;
			rea.s_to = 0;
		}
		*/

		Reaction& operator=(Reaction&& rea) = default;

		constexpr unsigned state_from() const noexcept{ return s_from; }
		constexpr unsigned state_to() const noexcept{ return s_to; }
		constexpr unsigned state_rate() const noexcept{ return transition_rate; }

	private:
		unsigned s_from;
		unsigned s_to;
		double transition_rate;
	};

	/*
	 * function wrappers based on transitions
	 */
	namespace transition {

		constexpr
		unsigned sink(const Reaction& r) noexcept
		{
			return r.state_from();
		};

		constexpr
		unsigned source(const Reaction& r) noexcept
		{
			return r.state_to();
		};

		constexpr
		double rate(const Reaction& r) noexcept
		{
			return r.state_rate();
		};
	} // end of r_transition_index namespace

	/*
	 *
	 */
	template <typename Type>
	class propensity_array {
	public:

		// [note to self - perfect forwarding may be beneficial here - profile first!]
		//
		//template <typename... Densities>
		//constexpr propensity_array (Densities&&... args) {
		//	arr.emplace(std::forward<Densities>(args)...);
		//}

		propensity_array (std::size_t S) :
			arr(S,0)
		{}

	private:
		std::vector<Type> arr;
		Type sum{0};
	};

	/*
	 *
	 */
	//template <unsigned nStates, unsigned nReactions>
	class Model
	{
		//template <unsigned r, unsigned c> using Reaction_matrix = std::array<std::array<unsigned, c>, r>;
		//template <unsigned r> using propensity_vector = std::array<double, r>;
	public:

		Model (double end_time_,
				std::initializer_list<unsigned> _states,
				std::initializer_list<Reaction> _Reac) :
			num_states(_states.size()),
			num_reactions(_Reac.size()),
			prop(num_reactions),
			output(num_states),
			reaction_table(static_cast<signed>(num_states*num_reactions),0),
			reaction_states_from(num_states),
			final_time(time_)
		{
			std::size_t counter {0};
			for (const auto& r : _Reac) {
				reaction_states_from[counter] = transition::sink(r);
				reaction_table[transition::sink(r) + counter*num_states] = -1;
				reaction_table[transition::source(r) + counter*num_states] = 1;
				reaction_rates[counter] = transition::rate(r);
				++counter;
			};
		};


		void operator ()() {};

	private:

		/*
		 * number of Markov states and reactions between them
		 */
		unsigned num_states;
		unsigned num_reactions;

		/*
		 * the density helper vector - for the Gillespie simulation
		 */
		propensity_array<double> prop;

		/*
		 * the state output
		 */
		std::vector<unsigned> output;

		/*
		 *the table of transition directions
		 */
		std::vector<signed> reaction_table;

		/*
		 * the reaction rates of the transitions between the states
		 */
		std::vector<double> reaction_rates;

		/*
		 * gather the indices of the state for reaction
		 */
		std::vector<unsigned> reaction_states_from;
	};

	/*
	 *
	 */
	template <typename Type>
	unsigned stochastic_state_step(std::vector<Type> c)
	{
		uniform_random_number<std::mt19937> generate_number;
		auto s = generate_number;

	}

} // end of namespace gillespie


#endif /* GILLESPIEGENERATOR_H_ */
