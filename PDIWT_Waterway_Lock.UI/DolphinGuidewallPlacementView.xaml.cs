using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PDIWT.Waterway.Lock.UI
{
    using ViewModel;
    using Model;
    /// <summary>
    /// Interaction logic for DolphinGuidewallPlacementView.xaml
    /// </summary>
    public partial class DolphinGuidewallPlacementView : UserControl
    {
        public DolphinGuidewallPlacementView()
        {
            InitializeComponent();
            DataContext = new DolphinGuidewallPlacementViewModel();
        }

        /// <summary>
        /// Gets the view's ViewModel.
        /// </summary>
        public DolphinGuidewallPlacementViewModel Vm
        {
            get
            {
                return (DolphinGuidewallPlacementViewModel)DataContext;
            }
        }

        public DolphinGuidewallPlacementData GetData() => Vm.SelectedData;
    }
}
