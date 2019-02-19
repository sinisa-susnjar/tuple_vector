		auto rt = cppbench::time(n_tests, {
			 { "vector", [ts,vec]() {
				size_t i = 0;
				for (const auto &it : ts)
					if (vec[i++].second != it.second)
						abort();
			}},
			{ "map",	[ts,&map]() {
				for (const auto &it : ts)
					if (map[it.first] != it.second)
						abort();
			}},
			{ "tuple",	[ts,tv]() {
				size_t i = 0;
				for (const auto &it : ts)
					if (tv[i++].second != it.second)
						abort();
			}}
		});
