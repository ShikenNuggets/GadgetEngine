using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Workbench
{
    public interface IMultiSelectComponent{}

    [DataContract]
    public abstract class ComponentVM : BaseViewModel
    {
        [DataMember] public GameObjectVM Owner { get; private set; }

        [DataMember] public static List<string> ComponentTypes
        {
            get
            {
                var list = GadgetAPI.GetDeclaredComponents();
                list.Sort();
                return list;
            }
        }

        public ComponentVM(GameObjectVM owner)
        {
            Debug.Assert(owner != null);
            Owner = owner;
        }

        public abstract IMultiSelectComponent GetMultiSelectComponent(MultiSelectedObjectVM msGo);
    }

    public abstract class MultiSelectComponentVM<T> : BaseViewModel, IMultiSelectComponent where T : ComponentVM
    {
        private bool _enableUpdates = true;

        public List<T> SelectedComponents { get; private set; }

        public MultiSelectComponentVM(MultiSelectedObjectVM msObj)
        {
            Debug.Assert(msObj != null);
            Debug.Assert(msObj.SelectedObjects != null);
            Debug.Assert(msObj?.SelectedObjects?.Any() == true);
            List<T?> selection = msObj.SelectedObjects.Select(obj => obj.GetComponent<T>()).ToList();
            selection.RemoveAll(x => x is null);
            SelectedComponents = selection.Where(x => x is not null).Select(x => x!).ToList();

            PropertyChanged += (s, e) =>
            {
                if (_enableUpdates && e != null && e.PropertyName != null)
                {
                    UpdateComponents(e.PropertyName);
                }
            };
        }

        protected abstract bool UpdateComponents(string propertyName);
        protected abstract bool UpdateMultiSelectComponent();

        public void Refresh()
        {
            _enableUpdates = false;
            UpdateMultiSelectComponent();
            _enableUpdates = true;
        }
    }
}
