#ifndef GADGET_WORKBENCH_FLOWS_EDITOR_FLOW_H
#define GADGET_WORKBENCH_FLOWS_EDITOR_FLOW_H

#include "Flows/Flow.h"

#include "Windows/EditorToolbar.h"

namespace Gadget::Workbench{
	class EditorFlow : public Flow{
	public:
		EditorFlow();
		virtual ~EditorFlow() override;

	private:
		EditorToolbar* editorToolbar;
	};
}

#endif //!GADGET_WORKBENCH_FLOWS_EDITOR_FLOW_H