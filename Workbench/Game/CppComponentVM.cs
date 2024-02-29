using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    public class CppComponentVM : ComponentVM
    {
        private List<GadgetAPIStructs.NamedVar> _variables;

        [DataMember]
        public List<GadgetAPIStructs.NamedVar> Variables
        {
            get => _variables;
            set
            {
                if (_variables != value)
                {
                    _variables = value;
                    OnPropertyChanged(nameof(Variables));
                }
            }
        }

        public CppComponentVM(GameObjectVM owner) : base(owner)
        {
            _variables = new List<GadgetAPIStructs.NamedVar>();
        }

        public override IMultiSelectComponent GetMultiSelectComponent(MultiSelectedObjectVM msGo) => new MultiSelectCppComponentVM(msGo);
    }

    public sealed class MultiSelectCppComponentVM : MultiSelectComponentVM<CppComponentVM>
    {
        public MultiSelectCppComponentVM(MultiSelectedObjectVM msObj) : base(msObj)
        {
            Refresh();
        }

        protected override bool UpdateComponents(string propertyName)
        {
            return false;
        }

        protected override bool UpdateMultiSelectComponent()
        {
            return true;
        }
    }
}
