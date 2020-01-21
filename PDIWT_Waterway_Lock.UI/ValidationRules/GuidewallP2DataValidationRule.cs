using System.Globalization;
using System.Windows.Controls;
using System.Windows.Data;

namespace PDIWT.Waterway.Lock.UI.ValidationRules
{
    using ViewModel;
    using Model;
    public class GuidewallP2DataValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            BindingGroup bindingGroup = value as BindingGroup;

            GuidewallP2ViewModel vm = bindingGroup.Items[0] as GuidewallP2ViewModel;
            //GuidewallP2Data data = vm.SelectedData;
            bool _selectedDataResult = bindingGroup.TryGetValue(vm, "SelectedData", out object _selectedData);
            if (!_selectedDataResult)
            {
                return new ValidationResult(false, "Properties not found");
            }
            GuidewallP2Data data = _selectedData as GuidewallP2Data;
            //bool dolphinTopEleResult = bindingGroup.TryGetValue(vm, "SelectedData.DolphinTopElevation", out object _dolphinTopElevation);
            //bool dolphinBottomEleResult = bindingGroup.TryGetValue(vm, "SelectedData.DolpinBottomElevation", out object _dolphinBottomElevation);

            //double _dolphinTopElevationNum = (double)_dolphinTopElevation;
            //double _dolphinBottomElevationNum = (double)_dolphinBottomElevation;

            if (data.DolphinBottomElevation >= data.DolphinTopElevation)
                return new ValidationResult(false, "Dolphin Bottom Elevation is equal or greater than its top Elevation");
            return ValidationResult.ValidResult;
        }
    }
}