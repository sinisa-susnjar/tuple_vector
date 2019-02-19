		auto rt = cppbench::time(n_tests, {
			{ "map",	[ts,map]() {
				K dt;
				K l = ts.crbegin()->first;
				for (auto &i : ts) {
					dt = i.first;
					dt++;
					if (dt > l)
						break;
					auto it = map.lower_bound(dt);
					if (it == map.end())
						abort();
					if ((*it).first < dt)
						abort();
				}
			}},
			{ "tuple",	[ts,&tv]() {
				K dt;
				K l = ts.crbegin()->first;
				for (auto &i : ts) {
					dt = i.first;
					dt++;
					if (dt > l)
						break;
					auto it = tv.lower_bound(dt);
					if (it == tv.end())
						abort();
					if ((*it).first < dt)
						abort();
				}
			}}
		});
