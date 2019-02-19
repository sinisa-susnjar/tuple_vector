		auto rt = cppbench::time(n_tests, {
			 { "vector", [ts,vec]() {
				auto it = ts.begin();
				for (auto &x : vec) {
					if (x.first != it->first)
						abort();
					++it;
				}
			}},
			{ "map",	[ts,map]() {
				auto x = map.begin();
				auto i = ts.begin();
				for (; i != ts.end(); ++i, ++x) {
					if (x->first != i->first)
						abort();
				}
			
			}},
			{ "tuple",	[ts,tv]() {
				auto x = tv.begin();
				auto i = ts.begin();
				for (; i != ts.end(); ++i, ++x) {
					if (x->first != i->first)
						abort();
				}
			}}
		});
