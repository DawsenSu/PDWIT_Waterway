using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;
using GalaSoft.MvvmLight.Messaging;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Xml.Serialization;
using BD = Bentley.DgnPlatformNET;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{
    using Model;
    public class PickupDolphinP11TemplateViewModel : ViewModelBase
    {
        public PickupDolphinP11TemplateViewModel(DolphinP11ViewModel VM)
        {
            if (!DeserializeDataCollection())
            {
                Datas = new ObservableCollection<DolphinP11Data>();
            }
            _vm = VM;
        }

        private DolphinP11ViewModel _vm;
        private string xmlFileName = "DolphinP11.xml";

        #region MVVMProperties

        private ObservableCollection<DolphinP11Data> _datas;

        /// <summary>
        /// Property Description
        /// </summary>
        public ObservableCollection<DolphinP11Data> Datas
        {
            get { return _datas; }
            set { Set(ref _datas, value); }
        }

        private DolphinP11Data _selectedData;

        /// <summary>
        /// Property Description
        /// </summary>
        public DolphinP11Data SelectedData
        {
            get { return _selectedData; }
            set { Set(ref _selectedData, value); }
        }

        #endregion MVVMProperties

        #region MVVMRelayCommands

        private RelayCommand _selectedTemplate;

        /// <summary>
        /// Gets the SelecteTemplate.
        /// </summary>
        public RelayCommand SelecteTempalte
        {
            get
            {
                return _selectedTemplate
                    ?? (_selectedTemplate = new RelayCommand(ExecuteSelecteTemplate,
                    () => _selectedData != null));
            }
        }

        private void ExecuteSelecteTemplate()
        {
            _vm.SelectedData = _selectedData;

            MessengerInstance.Send(new NotificationMessage(this, "CloseWindow"));
        }

        private RelayCommand _addToTemplates;

        /// <summary>
        /// Gets the AddToTemplates.
        /// </summary>
        public RelayCommand AddToTemplates
        {
            get
            {
                return _addToTemplates
                    ?? (_addToTemplates = new RelayCommand(ExecuteAddToTemplates));
            }
        }

        private void ExecuteAddToTemplates()
        {
            DolphinP11Data _addData = ObjectCopier.Clone(_vm.SelectedData);
            Datas.Add(_addData);
        }

        private RelayCommand _deleteFromTemplates;

        /// <summary>
        /// Gets the DeteleFromTemplates.
        /// </summary>
        public RelayCommand DeteleFromTemplates
        {
            get
            {
                return _deleteFromTemplates
                    ?? (_deleteFromTemplates = new RelayCommand(ExecuteDeteleFromTemplates,
                    () => _selectedData != null));
            }
        }

        private void ExecuteDeteleFromTemplates()
        {
            Datas.Remove(_selectedData);
        }

        private RelayCommand _windowClosed;

        /// <summary>
        /// Gets the WindowClosed.
        /// </summary>
        public RelayCommand WindowClosed
        {
            get
            {
                return _windowClosed
                    ?? (_windowClosed = new RelayCommand(
                    () =>
                    {
                        if (!SerializeDataCollection())
                            System.Windows.Forms.MessageBox.Show("DataCollection Can't be serialized", "ERROR", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                    }));
            }
        }

        private RelayCommand _saveAndExit;

        /// <summary>
        /// Gets the SaveAndExit.
        /// </summary>
        public RelayCommand SaveAndExit
        {
            get
            {
                return _saveAndExit
                    ?? (_saveAndExit = new RelayCommand(
                    () =>
                    {
                        MessengerInstance.Send(new NotificationMessage(this, "CloseWindow"));
                    }));
            }
        }

        #endregion MVVMRelayCommands

        /// <summary>
        /// Serialize data collection to workset xml file;
        /// </summary>
        /// <returns></returns>
        private bool SerializeDataCollection()
        {
            string worksetStandards = BD.ConfigurationManager.GetVariable("_USTN_WORKSETSTANDARDS");
            if (!Directory.Exists(worksetStandards))
            {
                MessageBox.Show($"_USTN_WORKSETSTANDARDS: {worksetStandards} doesn't exist.", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }
            XmlSerializer templateSerializer = new XmlSerializer(typeof(ObservableCollection<DolphinP11Data>));
            TextWriter sw = new StreamWriter(worksetStandards + xmlFileName);
            templateSerializer.Serialize(sw, _datas);
            sw.Close();
            return true;
        }

        /// <summary>
        /// Deserialize data collection from workset xml file;
        /// </summary>
        /// <returns></returns>
        private bool DeserializeDataCollection()
        {
            string worksetStandards = BD.ConfigurationManager.GetVariable("_USTN_WORKSETSTANDARDS");
            string xmlPath = worksetStandards + xmlFileName;

            if (File.Exists(xmlPath))
            {
                var templateDeserilazier = new XmlSerializer(typeof(ObservableCollection<DolphinP11Data>));
                var xmlFileStream = new FileStream(xmlPath, FileMode.Open);
                var datas = (ObservableCollection<DolphinP11Data>)templateDeserilazier.Deserialize(xmlFileStream);
                xmlFileStream.Close();

                if (datas != null)
                    Datas = datas;
                else
                    return false;
            }
            else
                return false;
            return true;
        }
    }
}