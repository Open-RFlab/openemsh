///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <filesystem>
#include <fstream>
#include <string>

#include "lpf.hpp"
#include "stub.hpp"

#include "infra/serializers/serializer_to_plantuml.hpp"

using namespace domain;

//******************************************************************************
SCENARIO("string SerializerToPlantuml::run(Board& board)", "[serializer_to_plantuml]") {
	// TODO This part might be better as 'e2e test' than 'unit test'.
	auto const auto_mesh = [](auto& board) {
		board->detect_edges_in_polygons();
		board->detect_colinear_edges();
		board->detect_non_conflicting_edges();
		board->auto_solve_all_edge_in_polygon();
		board->auto_solve_all_colinear_edges();
		board->detect_and_solve_too_close_meshline_policies();
		board->detect_intervals();
		board->mesh();
	};

	GIVEN("The Lpf complex structure") {
		std::shared_ptr<Board> lpf = create_lpf();
		auto params_state = lpf->global_params->get_current_state();
		params_state.lmin = 1;
		params_state.proximity_limit = 0;
//		params_state.dmax = 2;
		lpf->global_params->set_next_state(params_state);
		auto_mesh(lpf);

		WHEN("Serializing to Plantuml") {
			std::string puml = SerializerToPlantuml::run(*lpf);
			THEN("Should return some text") {
				REQUIRE_FALSE(puml.empty());
			}

			AND_WHEN("Writing it to a file") {
				std::ofstream out(QRFL_UNITTEST_DIR"/lpf.puml");
				out << puml;
				THEN("Should create a file") {
					INFO(QRFL_UNITTEST_DIR "/lpf.puml");
					REQUIRE(std::filesystem::is_regular_file(QRFL_UNITTEST_DIR "/lpf.puml"));
				}
			}
		}
	}

	// TODO This part might be better as 'e2e test' than 'unit test'.
	GIVEN("The Stub complex structure") {
		std::shared_ptr<Board> stub = create_stub();
		auto params_state = stub->global_params->get_current_state();
		params_state.lmin = 0;
		params_state.proximity_limit = 0.1;
//		params_state.dmax = 2;
		stub->global_params->set_next_state(params_state);
		auto_mesh(stub);

		WHEN("Serializing to Plantuml") {
			std::string puml = SerializerToPlantuml::run(*stub);
			THEN("Should return some text") {
				REQUIRE_FALSE(puml.empty());
			}

			AND_WHEN("Writing it to a file") {
				std::ofstream out(QRFL_UNITTEST_DIR"/stub.puml");
				out << puml;
				THEN("Should create a file") {
					INFO(QRFL_UNITTEST_DIR "/stub.puml");
					REQUIRE(std::filesystem::is_regular_file(QRFL_UNITTEST_DIR "/stub.puml"));
				}
			}
		}
	}
}
