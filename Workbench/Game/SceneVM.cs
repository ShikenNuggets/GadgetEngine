﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Workbench
{
    [DataContract]
    public class SceneVM : BaseViewModel
    {
        [DataMember (Name="Name")] private string _name;
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

        [DataMember] public ProjectVM Project { get; private set; }

        [DataMember (Name="IsActive")] private bool _isActive;
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

        [DataMember (Name = nameof(GameObjects))] private ObservableCollection<GameObjectVM> _gameObjects = new ObservableCollection<GameObjectVM>();
        public ReadOnlyObservableCollection<GameObjectVM> GameObjects { get; private set; }

        public ICommand AddGameObjectCommand {  get; private set; }
        public ICommand RemoveGameObjectCommand {  get; private set; }

        public SceneVM(ProjectVM project, string name)
        {
            Debug.Assert(project != null);
            Debug.Assert(!string.IsNullOrWhiteSpace(name));

            Project = project;
            Name = name;

            OnDeserialized(new StreamingContext());
        }

        private GameObjectVM AddGameObject(GameObjectVM gameObject)
        {
            Debug.Assert(gameObject != null);
            Debug.Assert(!_gameObjects.Contains(gameObject));
            _gameObjects.Add(gameObject);
            return gameObject;
        }

        private void RemoveGameObject(GameObjectVM gameObject)
        {
            Debug.Assert(gameObject != null);
            Debug.Assert(_gameObjects.Contains(gameObject));
            _gameObjects.Remove(gameObject);
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_gameObjects == null)
            {
                //TODO - Indicate some kind of "corrupted project" error to the user
                _gameObjects = new ObservableCollection<GameObjectVM>();
            }

            GameObjects = new ReadOnlyObservableCollection<GameObjectVM>(_gameObjects);
            OnPropertyChanged(nameof(GameObjects));

            AddGameObjectCommand = new RelayCommand<GameObjectVM>(x =>
            {
                Debug.Assert(x != null);
                AddGameObject(x);
                var index = _gameObjects.IndexOf(x);

                ProjectVM.UndoRedo.Add(new UndoRedoAction(
                    $"Added '{x.Name}' to Scene '{Name}'",
                    () => RemoveGameObject(x),
                    () => _gameObjects.Insert(index, x)
                ));
            });

            RemoveGameObjectCommand = new RelayCommand<GameObjectVM>(x =>
            {
                Debug.Assert(x != null);
                var index = _gameObjects.IndexOf(x);
                RemoveGameObject(x);

                ProjectVM.UndoRedo.Add(new UndoRedoAction(
                    $"Removed '{x.Name}' from Scene '{Name}'",
                    () => _gameObjects.Insert(index, x),
                    () => RemoveGameObject(x)
                ));
            });
        }
    }
}