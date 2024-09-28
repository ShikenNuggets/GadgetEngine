#ifndef GADGET_WORKBENCH_FLOWS_FLOW_H
#define GADGET_WORKBENCH_FLOWS_FLOW_H

namespace Gadget::Workbench{
	class Flow{
	public:
		Flow() : isFlowComplete(false){}

		bool IsFlowComplete() const{ return isFlowComplete; }
		virtual Flow* NextFlow(){ return nullptr; }

	protected:
		bool isFlowComplete;
	};
}

#endif //!GADGET_WORKBENCH_FLOWS_FLOW_H