/*
 * BUGS:
 * Bunker has physical panel objects which I can't move :(
 * Shipwreck vault does not copy well
 * Pillars can be swapped but they don't zoom you out enough. Maybe SWAP_DISTANCE?
 * FEATURES:
 * SWAP_TARGETS should still require the full panel sequence (and have ways to prevent softlocks?)
 * Randomize audio logs
 * Separate panels out by "square", which can probably be anywhere
 * List of panels which can be valid "Burn" (aka desert) substitutes: Prevent (or limit) panel counting
 * List of panels which split left/right (for left/right controls)
 * List of panels which split up/down (for up/down controls)
 * Swap sounds in jungle (along with panels) -- maybe impossible
 * Make orange 7 (all of oranges?) hard. Like big = hard.
 * TRY:
 * Swap treehouse pivots
*/
#include "Memory.h"
#include "WitnessRandomizer.h"
#include "Panels.h"
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
	WitnessRandomizer randomizer = WitnessRandomizer();

	if (argc == 2) {
		srand(atoi(argv[1])); // Seed with RNG from command line
	}

	// randomizer.Randomize(bunkerPanels,			SWAP_LINES | SWAP_STYLE);
	/*
	randomizer.Randomize(lasers,				SWAP_TARGETS);
	randomizer.Randomize(tutorialPanels,		SWAP_TARGETS);
	randomizer.Randomize(monasteryPanels,		SWAP_TARGETS);
	randomizer.Randomize(shadowsPanels,			SWAP_TARGETS);

	randomizer.Randomize(desertPanels,			SWAP_LINES);
	randomizer.Randomize(treehousePanels,		SWAP_LINES);

	std::vector<int> squarePanels;
	for (auto panelSet : {outsideTutorialPanels, symmetryPanels, quarryPanels, keepPanels, townPanels, bunkerPanels, swampPanels, mountainPanels, utmPanels}) {
		squarePanels.insert(squarePanels.end(), panelSet.begin(), panelSet.end());
	}
	randomizer.Randomize(squarePanels,			SWAP_LINES | SWAP_STYLE);

	randomizer.Randomize(junglePanels,			SWAP_LINES | SWAP_STYLE);
	randomizer.Randomize(mountainMultipanel,	SWAP_LINES | SWAP_STYLE);
	randomizer.Randomize(pillars,				SWAP_LINES | SWAP_STYLE | SWAP_BACK_DISTANCE);
	/*/
	int BOATH_3_1 = 0x21B5;
	int MILL_L_1 = 0xE0C;
	int MILL_U_1 = 0x557;
	int QUARRY_E_1 = 0x9E57;
	int QUARRY_E_2 = 0x17C09;
	int MILL_E_1 = 0x1E5A;
	int BUNKER_G_1 = 0xA010;
	int BUNKER_T_1 = 0x9F7D;
	int TUT_PILLAR = 0xC335;
	int TUT_F_C = 0x293;
	int PILLAR_L_1 = 0x383D;
	int PILLAR_L_4 = 0x339BB;
	int PILLAR_C = 0x9DD5;

	randomizer.SwapPanels(PILLAR_L_1, PILLAR_C, SWAP_LINES | SWAP_STYLE | SWAP_BACK_DISTANCE);
	//*/
}

WitnessRandomizer::WitnessRandomizer() : _memory(Memory("witness64_d3d11.exe"))
{
	// Turn off desert flood final
	_memory.WriteData<float>({0x5B28C0, 0x18, 0x18076*8, 0x2A8}, {0.0f, 0.0f});
	// Change desert floating target to desert flood final
	_memory.WriteData<int>({0x5B28C0, 0x18, 0x17ECA*8, 0x2BC}, {0x18077});
}

void WitnessRandomizer::Randomize(std::vector<int> panels, int flags) {
	if (panels.size() == 0) return;
	for (size_t i=panels.size() - 1; i > 1; i--) {
		int target = rand() % i;
		if (i != target) {
			// std::cout << "Swapping panels " << std::hex << panels[i] << " and " << std::hex << panels[target] << std::endl;
			SwapPanels(panels[i], panels[target], flags);
			std::swap(panels[i], panels[target]); // Panel indices in the array
		}
	}
}

void WitnessRandomizer::SwapPanels(int panel1, int panel2, int flags) {
	std::map<int, int> offsets;

	if (flags & SWAP_TARGETS) {
		offsets[0x2BC] = sizeof(int);
	}
	if (flags & SWAP_LINES) {
		offsets[0x230] = 16; // traced_edges
//		offsets[0x220] = sizeof(void*); // *pattern_name
//		offsets[0x240] = sizeof(void*); // *mesh_name
		offsets[0x2FC] = sizeof(int); // is_cylinder
		offsets[0x300] = sizeof(float); // cylinder_z0
		offsets[0x304] = sizeof(float); // cylinder_z1
		offsets[0x308] = sizeof(float); // cylinder_radius
//		offsets[0x35C] = sizeof(int); // solvable_from_behind

//		offsets[0x30C] = sizeof(float); // uv_to_world_scale
		offsets[0x3A4] = sizeof(float); // path_width_scale
		offsets[0x3A8] = sizeof(float); // startpoint_scale
		offsets[0x3B8] = sizeof(int); // num_dots
		offsets[0x3BC] = sizeof(int); // num_connections
		offsets[0x3C8] = sizeof(void*); // *dot_positions
		offsets[0x3D0] = sizeof(void*); // *dot_flags
		offsets[0x3D8] = sizeof(void*); // *dot_connection_a
		offsets[0x3E0] = sizeof(void*); // *dot_connection_b
		offsets[0x420] = sizeof(void*); // *decorations
		offsets[0x428] = sizeof(void*); // *decoration_flags
		offsets[0x438] = sizeof(int); // num_decorations
		offsets[0x440] = sizeof(void*); // *reflection_data
		offsets[0x448] = sizeof(int); // grid_size_x
		offsets[0x44C] = sizeof(int); // grid_size_y
		offsets[0x450] = sizeof(int); // style_flags // This is required to not ignore dots
		offsets[0x45C] = sizeof(int); // sequence_len
		offsets[0x460] = sizeof(void*); // *sequence
		offsets[0x468] = sizeof(int); // dot_sequence_len
		offsets[0x470] = sizeof(void*); // *dot_sequence
		offsets[0x478] = sizeof(int); // dot_sequence_len_reflection
		offsets[0x480] = sizeof(void*); // *dot_sequence_reflection
		offsets[0x4B0] = sizeof(void*); // *panel_target
		offsets[0x4D8] = sizeof(void*); // *specular_texture
	}
	if (flags & SWAP_STYLE) {
		offsets[0xC8] = 16; // path_color
		offsets[0xD8] = 16; // reflection_path_color
//		offsets[0xE8] = 16; // deprecated_finished_path_color
		offsets[0xF8] = 16; // dot_color
		offsets[0x108] = 16; // active_color
		offsets[0x118] = 16; // background_region_color
		offsets[0x128] = 16; // success_color_a
		offsets[0x138] = 16; // success_color_b
		offsets[0x148] = 16; // strobe_color_a
		offsets[0x158] = 16; // strobe_color_b
		offsets[0x168] = 16; // error_color
//		offsets[0x178] = 16; // video_status_color
		offsets[0x188] = 16; // pattern_point_color
		offsets[0x198] = 16; // pattern_point_color_a
		offsets[0x1A8] = 16; // pattern_point_color_b
		offsets[0x1B8] = 16; // symbol_a
		offsets[0x1C8] = 16; // symbol_b
		offsets[0x1D8] = 16; // symbol_c
		offsets[0x1E8] = 16; // symbol_d
		offsets[0x1F8] = 16; // symbol_e
		offsets[0x208] = sizeof(int); // push_symbol_colors
		offsets[0x20C] = 16; // outer_background
		offsets[0x21C] = sizeof(int); // outer_background_mode
		offsets[0x278] = sizeof(void*); // *audio_prefix
		offsets[0x430] = sizeof(void*); // *decoration_colors
		offsets[0x4A0] = sizeof(int); // num_colored_regions
		offsets[0x4A8] = sizeof(void*); // *colored_regions
//		offsets[0x4B8] = sizeof(void*); // *backing_texture
	}
	if (flags & SWAP_BACK_DISTANCE) {
		offsets[0x22C] = sizeof(float); // extra_back_distance
	}

	for (auto const& [offset, size] : offsets) {
		SwapPanelData(panel1, panel2, offset, size);
	}
}

void WitnessRandomizer::SwapPanelData(int panel1, int panel2, int finalOffset, int dataSize) {
	// Currently wired for old version
	std::vector<int> panel1Offset = {0x5B28C0, 0x18, panel1*8, finalOffset};
	std::vector<int> panel2Offset = {0x5B28C0, 0x18, panel2*8, finalOffset};

	std::vector<byte> panel1Data = _memory.ReadData<byte>(panel1Offset, dataSize);
	std::vector<byte> panel2Data = _memory.ReadData<byte>(panel2Offset, dataSize);

	_memory.WriteData<byte>(panel2Offset, panel1Data);
	_memory.WriteData<byte>(panel1Offset, panel2Data);
}
