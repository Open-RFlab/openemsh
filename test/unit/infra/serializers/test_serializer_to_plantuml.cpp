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
	GIVEN("The Lpf complex structure") {
		std::shared_ptr<Board> lpf = create_lpf();
		lpf->params.lmin = 1;
		lpf->params.proximity_limit = 0;
//		lpf->params.dmax = 2;
		lpf->auto_mesh();

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
		stub->params.lmin = 0;
		stub->params.proximity_limit = 0.1;
//		stub->params.dmax = 2;
		stub->auto_mesh();

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
