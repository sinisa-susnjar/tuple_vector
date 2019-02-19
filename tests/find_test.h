		auto rt = cppbench::time(n_tests, {
			{ "map",	[ts,map]() {
				for (auto &i : ts) {
					auto it = map.find(i.first);
					if (it->first != i.first)
						abort();
				}
			}},
			{ "tuple",	[ts,&tv]() {
				for (auto &i : ts) {
					auto it = tv.find(i.first);
					if (it == tv.end())
						abort();
					if ((*it).first != i.first)
						abort();
				}
			}}
		});
