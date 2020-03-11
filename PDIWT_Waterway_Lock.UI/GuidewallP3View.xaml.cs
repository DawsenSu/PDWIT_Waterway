using PDIWT.Waterway.Lock.UI.ViewModel;
using System.Windows.Controls;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for GuidewallP3View.xaml
    /// </summary>
    public partial class GuidewallP3View : UserControl
    {
        public GuidewallP3View(BM.AddIn addIn)
        {
            InitializeComponent();
            DataContext = new GuidewallP3ViewModel(addIn);
        }
        /// <summary>
        /// Gets the view's ViewModel.
        /// </summary>
        public GuidewallP3ViewModel Vm
        {
            get
            {
                return (GuidewallP3ViewModel)DataContext;
            }
        }

        public Model.GuidewallP3Data GetData()
        {
            return Vm.SelectedData;
        }
    }
}
