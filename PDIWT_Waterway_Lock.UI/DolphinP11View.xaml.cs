using PDIWT.Waterway.Lock.UI.ViewModel;
using System.Windows.Controls;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for DolphinP11View.xaml
    /// </summary>
    public partial class DolphinP11View : UserControl
    {
        public DolphinP11View(BM.AddIn addIn)
        {
            InitializeComponent();
            DataContext = new DolphinP11ViewModel(addIn);
        }

        /// <summary>
        /// Gets the view's ViewModel.
        /// </summary>
        public DolphinP11ViewModel Vm
        {
            get
            {
                return (DolphinP11ViewModel)DataContext;
            }
        }

        public Model.DolphinP11Data GetData()
        {
            return Vm.SelectedData;
        }
    }
}
