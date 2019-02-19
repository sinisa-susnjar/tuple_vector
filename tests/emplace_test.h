		auto rt = cppbench::time(n_tests, {
			{ "tuple",	[ts,&tv]() {
				tv.clear();
				tv.reserve(ts.size());
				for (const auto &it : ts)
					tv.emplace_back(it);
			}},
			{ "vector", [ts,&vec]() {
				vec.clear();
				vec.reserve(ts.size());
				for (const auto &it : ts)
					vec.emplace_back(it);
			}},
			{ "map",	[ts,&map]() {
				map.clear();
				for (const auto &it : ts)
					map.emplace_hint(map.end(), it.first, it.second);
			
			}}
		});
