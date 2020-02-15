using PDIWT.Waterway.Lock.UI.ViewModel;
using System.Windows.Controls;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for GuidewalP2View.xaml
    /// </summary>
    public partial class GuidewallP2View : UserControl
    {
        public GuidewallP2View(BM.AddIn addIn)
        {
            InitializeComponent();
            DataContext = new GuidewallP2ViewModel(addIn);
        }

        /// <summary>
        /// Gets the view's ViewModel.
        /// </summary>
        public GuidewallP2ViewModel Vm
        {
            get
            {
                return (GuidewallP2ViewModel)DataContext;
            }
        }

        public Model.GuidewallP2Data GetData()
        {
            return Vm.SelectedData;
        }
    }
}