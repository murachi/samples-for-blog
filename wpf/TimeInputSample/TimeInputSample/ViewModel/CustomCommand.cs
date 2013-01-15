using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace TimeInputSample.ViewModel
{
    /// <summary>
    /// コマンドを作るためのクラス。
    /// </summary>
    /// <remarks>
    /// コマンドの内容ごとにクラスを作るのは手間がかかるので、
    /// コンストラクタにデリゲートを渡してコマンドオブジェクトを作っちゃうクラスを
    /// 作っておくと楽ちん、らしいですよ。
    /// 
    /// 基本的にはこの辺を参考にしていますよ。
    /// http://code.msdn.microsoft.com/windowsdesktop/MVVM-d8261534
    /// </remarks>
    public class CustomCommand : ICommand
    {
        private Action<object> execute_function;
        private Func<object, bool> can_execute_function;

        /// <summary>
        /// コマンドの実行可否状態が変化したときに発生するイベント。
        /// </summary>
        /// <remarks>
        /// 詳しくは MSDN を見てくださいね。デフォルトだと実装がないので
        /// NotSupportedException とか飛ばすらしいですよ。
        /// CommandManager.RequerySuggested イベントに丸投げするのが定石、みたいですね。
        /// </remarks>
        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        /// <param name="exec_func">実行時に呼び出して欲しい処理のデリゲート。</param>
        /// <param name="can_exec_func">実行可否状態を判定するためのデリゲート。</param>
        public CustomCommand(Action<object> exec_func, Func<object, bool> can_exec_func)
        {
            execute_function = exec_func;
            can_execute_function = can_exec_func;
        }

        /// <summary>
        /// コンストラクタ。
        /// </summary>
        /// <param name="exec_func">実行時に呼び出して欲しい処理のデリゲート。</param>
        /// <remarks>
        /// いつでも実行可能なコマンドにしたい場合に使うコンストラクタですよ。
        /// </remarks>
        public CustomCommand(Action<object> exec_func) : this(exec_func, (parameter) => true) { }

        /// <summary>
        /// 実行時処理。
        /// </summary>
        /// <param name="parameter">パラメータとして任意に渡されるオブジェクト。</param>
        /// <remarks>
        /// システムが、コマンドを実行するよ、っていうときに呼び出すメソッドですよ。
        /// 単に、コンストラクタの第1引数に渡したデリゲートを、パラメータも丸投げで
        /// そのまま呼び出しているだけですよ。
        /// 
        /// ちなみに、 parameter に渡されるのは、 XAML で Button 要素とかの
        /// CommandParameter 属性に指定したオブジェクト、らしいですよ。
        /// </remarks>
        public void Execute(object parameter)
        {
            execute_function(parameter);
        }

        /// <summary>
        /// 実行可否状態の判定処理。
        /// </summary>
        /// <param name="parameter">パラメータとして任意に渡されるオブジェクト。</param>
        /// <returns>実行可能なら true を返す。</returns>
        /// <remarks>
        /// システムが、今本当に実行可能なの? というのを確認したいときに呼び出すメソッドですよ。
        /// CanExecuteChanged イベントは CommandManager.RequerySuggested イベントにぶら下げたので、
        /// CommandManager.RequerySuggested イベントが発生したときに呼ばれることになる、みたいですよ。
        /// でもそれって、どんなとき、なんでしょうかね?
        /// 参考にしたサイトの管理人さんも、よく知らない、らしいですよ。
        /// </remarks>
        public bool CanExecute(object parameter)
        {
            return can_execute_function(parameter);
        }
    }
}
