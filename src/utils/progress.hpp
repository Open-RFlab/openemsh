///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

//******************************************************************************
class Progress {
public:

	//**************************************************************************
	class IBar {
	public:
		virtual ~IBar() = default;
		virtual void tick(std::size_t i) = 0;
		virtual void tick(std::size_t i, std::size_t j) = 0;
		virtual void complete() = 0;
	};

	//**************************************************************************
	class Bar final : public IBar {
	private:
		std::vector<std::unique_ptr<IBar>> impls;

	public:
		static std::tuple<Progress::Bar, std::size_t, std::size_t> build(std::size_t max, std::string const& message);

		Bar(std::size_t max, std::string const& message);

		void tick(std::size_t i) override;
		void tick(std::size_t i, std::size_t j) override;
		void complete() override;
	};

	static Progress& singleton();
	void register_impl_builder(std::function<std::unique_ptr<IBar> (std::size_t, std::string const&)> const& builder);

private:
	std::vector<std::function<std::unique_ptr<IBar> (std::size_t, std::string const&)>> impl_builders;
};
