using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    public interface IUndoRedo
    {
        string Name { get; }

        void Undo();
        void Redo();
    }

    public class UndoRedoAction : IUndoRedo
    {
        private Action _undoAction;
        private Action _redoAction;

        public string Name { get; }

        public void Undo() => _undoAction?.Invoke();
        public void Redo() => _redoAction?.Invoke();

        public UndoRedoAction(string name, Action undo, Action redo)
        {
            Debug.Assert(undo != null && redo != null);
            Name = name;
            _undoAction = undo;
            _redoAction = redo;
        }

        public UndoRedoAction(string name, string property, object instance, object undoValue, object redoValue)
        {
            Debug.Assert(instance != null);
            Debug.Assert(!string.IsNullOrWhiteSpace(property));
            Debug.Assert(instance.GetType().GetProperty(property) != null);
            Debug.Assert(undoValue.GetType() == redoValue.GetType());

            Name = name;
            _undoAction = () => instance.GetType().GetProperty(property).SetValue(instance, undoValue);
            _redoAction = () => instance.GetType().GetProperty(property).SetValue(instance, redoValue);
        }
    }

    public class UndoRedo
    {
        private bool _canAddNewActions = true; //Used to prevent undoing/redoing *while* undoing/redoing

        private readonly ObservableCollection<IUndoRedo> _undoList = new ObservableCollection<IUndoRedo>();
        private readonly ObservableCollection<IUndoRedo> _redoList = new ObservableCollection<IUndoRedo>();

        public ReadOnlyObservableCollection<IUndoRedo> UndoList { get; private set; }
        public ReadOnlyObservableCollection<IUndoRedo> RedoList { get; private set; }

        public UndoRedo()
        {
            UndoList = new ReadOnlyObservableCollection<IUndoRedo>(_undoList);
            RedoList = new ReadOnlyObservableCollection<IUndoRedo>(_redoList);
        }

        public void Reset()
        {
            _undoList.Clear();
            _redoList.Clear();
        }

        public void Add(IUndoRedo cmd)
        {
            if (_canAddNewActions)
            {
                _undoList.Add(cmd);
                _redoList.Clear();

                Logger.Log(MessageType.Verbose, cmd.Name);
            }
        }

        public void Undo()
        {
            if (_undoList.Any())
            {
                var cmd = _undoList.Last();
                _undoList.Remove(cmd);
                _canAddNewActions = false;
                cmd.Undo();
                _canAddNewActions = true;
                _redoList.Insert(0, cmd);

                Logger.Log(MessageType.Verbose, "[Undo] " + cmd.Name);
            }
        }

        public void Redo()
        {
            if (_redoList.Any())
            {
                var cmd = _redoList.First();
                _redoList.Remove(cmd);
                _canAddNewActions = false;
                cmd.Redo();
                _canAddNewActions = true;
                _undoList.Add(cmd);

                Logger.Log(MessageType.Verbose, "[Redo] " + cmd.Name);
            }
        }
    }
}