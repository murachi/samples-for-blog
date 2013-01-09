using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace TimeInputSample.ViewModel
{
    /// <summary>
    /// View Model クラスの基底クラス。
    /// </summary>
    public class ViewModelBase : INotifyPropertyChanged
    {
        /// <summary>
        /// プロパティが変更されましたイベント。
        /// </summary>
        /// <remarks>
        /// こいつから派生するクラスのインスタンスが View にバインドされると、
        /// View の表示を更新する処理がこのイベントに勝手に関連づけられるらしいですよ。
        /// </remarks>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// プロパティの変更を通知する。
        /// </summary>
        /// <param name="property_name">プロパティ名</param>
        /// <remarks>
        /// 指定したプロパティ名のプロパティをバインドした表示属性の表示を更新したいときに
        /// こいつを呼べばいい、らしいですよ。
        /// </remarks>
        protected void NotifyPropertyChanged(string property_name)
        {
            if (PropertyChanged == null) return;
            PropertyChanged(this, new PropertyChangedEventArgs(property_name));
        }
    }
}
