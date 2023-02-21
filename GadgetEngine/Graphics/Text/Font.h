#ifndef GADGET_FONT_H
#define GADGET_FONT_H

#include "Graphics/Loaders/FontLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Font : public Resource{
	public:
		Font(){}
		virtual ~Font() override{}
	};

	class FontResourceContainer : public ResourceContainer{
		FontResourceContainer(const std::string& path_) : path(path_){}

		virtual Resource* LoadResource() override{
			return FontLoader::LoadFont(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Font);
		}

	private:
		std::string path;
	};
}

#endif //!GADGET_FONT_H