#ifndef DB_BUILD_OPTIONS_H
#define DB_BUILD_OPTIONS_H

namespace DB{
	struct BuildOptions{
		BuildOptions(int argc, char* argv[]);

		bool forceRebuild = false; //Force a full data rebuild even if it's technically not necessary
	};
}

#endif //!DB_BUILD_OPTIONS_H