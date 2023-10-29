using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    [DataContract]
    public class SceneVM : BaseViewModel
    {
        private string _name;
        public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        public ProjectVM Project { get; private set; }

        [DataMember] private bool _isActive;
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if (_isActive != value)
                {
                    _isActive = value;
                    OnPropertyChanged(nameof(IsActive));
                }
            }
        }

        public SceneVM(ProjectVM project, string name)
        {
            Debug.Assert(project != null);
            Debug.Assert(!string.IsNullOrWhiteSpace(name));

            Project = project;
            Name = name;
        }
    }
}
