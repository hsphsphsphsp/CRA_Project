#pragma once

#define R 0
#define W 1
#define E 2

#define HEX 0
#define DEC 1

#define CMD_BUFFER_MAP unordered_map<pair<int, unsigned int>, unsigned int, pair_hash>

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ (hash2 << 1);
	}
};