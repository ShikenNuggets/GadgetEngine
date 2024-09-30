#ifndef GADGET_WORKBENCH_CORE_FONT_MANAGER_H
#define GADGET_WORKBENCH_CORE_FONT_MANAGER_H

#include <Data/HashTable.h>

struct ImFont;

namespace Gadget::Workbench{
	class FontManager{
	public:
		FontManager();

		void Add(StringID name_, const String& filePath_);
		ImFont* Get(StringID name_) const;

	private:
		HashTable<StringID, ImFont*> fonts;
	};
}

#endif //!GADGET_WORKBENCH_CORE_FONT_MANAGER_H