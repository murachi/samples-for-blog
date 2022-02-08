# 基本的なウェブアプリケーションを構築するチュートリアルをやってみた

https://aws.amazon.com/jp/getting-started/hands-on/build-web-app-s3-lambda-api-gateway-dynamodb/

## 実装: Amplify コンソールでウェブアプリを作成
- [mod1-build-amplify-app ディレクトリへ](mod1-build-amplify-app/)

> 3. HTML ファイルのみを ZIP (圧縮) します。

アーカイバは何使ってもいいのか? とりあえず 7zip 形式を使ってみる。

> 6. [Git プロバイダーなしでデプロイ] を選択します。以下のように画面に表示されるはずです。

UI 全然違った(´・\_・\`)。
![Amplify 開始の実際の UI…。](mod1-build-amplify-app/figs/fig-ui-start-amplify.png)

Git 使わない方法だからとりあえず `Amplify Studio` を選んでおけばいいのかな?

> 8. [App 名] フィールドに、「GettingStarted」と入力します。

`build-amplify-app-tutorial` という名前にしてみた。

バックエンド環境の構築はまあまあ時間がかかる様子… 1〜2分程度で完了。

> 9. 環境名には、「dev」と入力します。

どこに入力するんだ? (´・\_・\`) つか、すでに staging になっちゃってるっぽいが? (´・\_・\`)
![Amplify バックエンド構築直後の UI](mod1-build-amplify-app/figs/fig-ui-amplify-after-build-backend.png)

...わからんので一旦削除して最初からやり直してみることにした。
![一旦削除](mod1-build-amplify-app/figs/fig-delete-amplify-app.png)

「使用を開始する」の画面に戻ったので、今度は `Amplify Hosting` を選んでみた。

チュートリアルどおりの UI だー。こっちでよかったのか…
![チュートリアルどおりの UI が出てきた](mod1-build-amplify-app/figs/fig-ui-start-with-amplify-hosting.png)

で、ドラッグ&amp;ドロップで `.7z` のファイルを放り込んだらこうなりました。どうやらサポートしてないらしい…(´・\_・\`)
![7zは駄目でした…](mod1-build-amplify-app/figs/fig-7z-not-supported.png)

`.tar.xz` も試してみたが駄目だったので、仕方なく `.zip` にして放り込む (`.zip` はファイル名 Unicode に対応してないから避けたいんだけどね…)。で、「保存してデプロイ」でやっと先に進みました。

やり方さえ分かっちゃえば静的コンテンツを公開する方法としてはめっちゃ簡単だ罠。

## 実装: Lambda 関数を作成して設定
- [mod2-create-lambda-function ディレクトリへ](mod2-create-lambda-function/)

> 2. 必ず、関数を作成しているリージョンをメモしてください。これは、ページの最上部、アカウント名の横にあります。

オレゴンでした。

> 4. [関数名] で、「HelloWorldFunction」と入力します。

`myTutorialFunction` としてみました。

> 5. [ランタイム] ドロップダウンから [Node.js 12.x] を選択します。

最新に合わせて `Node.js 14.x` を選択。

![Lambda の基本的な設定](mod2-create-lambda-function/figs/fig-lambda-base-settings.png)

> 9. 画面上部にあるオレンジの [保存] ボタンをクリックします。

オレンジのボタンは「Test」でした。そして先に「Deploy」ボタンを押すのが正解だったっぽい?

![コード入力枠の上にあるオレンジのボタン…](mod2-create-lambda-function/figs/fig-orange-button-on-code-area.png)

実際先に「Test」を押すと、テスト用の Lambda 関数を設定するフォームが出てきて、関数名とテスト用のパラメータ (request body となる JSON) の設定を要求される。関数名は同じく `myTutorialFunction` としてみた。(このへん画像を取り損なった…)

ただそれだけだと何故かデフォルトの Hello world 関数みたいなのが設定されちゃうので、ここで「Deploy」をクリックすると、先に設定したテスト用 Lambda 関数にコードの内容がデプロイされて、やっと「Test」ボタンでテストできるようになる。

![Test を実行したところ](mod2-create-lambda-function/figs/fig-lambda-test-executed.png)

## サーバレス関数にリンクする
- [mod3-create-REST-API ディレクトリへ](mod3-create-REST-API/)

まず始める前にリージョンを確認したほうが良さげ。 API Gateway のページに入った段階で、リージョンの設定が↑と違うなら選択し直す。わい、ここでバージニア北部になってるのに気づかずに進めちゃってめっさ手戻りした… orz

![リージョンはちゃんと確認しよう](mod3-create-REST-API/figs/fig-api-gateway-region.png)

### 実装: 新しい REST API を作成する
> 2. オレンジの [API の作成] ボタンをクリックします。
> 3. REST API ボックスを見つけて、その中のオレンジの [構築] ボタンをクリックします。

2 は無かった。3 はこれのことかな?

![API Gateway の最初のページ、の左下にあったブロック](mod3-create-REST-API/figs/fig-api-gateway-1st-page.png)

なんかアラートが出た。インポートフォーム? にサンプルプログラムをとりあえず突っ込んどいたって話?

![のっけからよくわからんアラート](mod3-create-REST-API/figs/fig-api-gateway-1st-alert.png)

> 4. [プロトコルの選択] で、[REST] を選択します。
> 4. [Create new API] で、[New API] を選択します。
> 4. [API 名] フィールドに、「HelloWorldAPI」と入力します。
> 4. [エンドポイントタイプ] プルダウンで [エッジ最適化] を選択します

API名は `myTutorialAPI` としてみた。

![API作成時の設定内容](mod3-create-REST-API/figs/fig-api-gateway-create-setting.png)

### 実装: 新しいリソースとメソッドを作成する
> 2. [/] リソースを選択した状態で、[アクション] ドロップダウンメニューから [メソッドの作成] をクリックします。

これか。

![「アクション」ドロップダウンメニューの「メソッドの作成」](mod3-create-REST-API/figs/fig-api-gateway-create-method.png)

> 3. 表示される新しいドロップダウンから POST を選択し、チェックマークをクリックします。

これのことらしい。(ドロップ開いた状態ではキャプチャが上手く取れなかった…)

![HTTPメソッドの選択](mod3-create-REST-API/figs/fig-api-gateway-method-http-method.png)

> 4. 統合タイプとして [Lambda 関数] を選択します。
> 4. [関数] フィールドに、「HelloWorldFunction」と入力します。
> 4. ブルーの [保存] ボタンをクリックします。

関数フィールドに、前のチュートリアルで作った関数名 `myTutorialFunction` を入力しようとすると、ちゃんと保管候補が出てくる。入力後の画面はこんな感じ。

![リソースの設定内容](mod3-create-REST-API/figs/fig-api-gateway-resource-setting.png)

> 7. 作成している API に Lambda 関数を呼び出すアクセス許可を与えていることを知らせるメッセージが表示されます。[OK] ボタンをクリックします。

Lambda 関数に権限をつけちゃうよ、というアラートが出た。

![リソースの設定内容](mod3-create-REST-API/figs/fig-api-gateway-alert-authority.png)

> 8. 新しく作成した POST メソッドを選択した状態で、[アクション] ドロップダウンメニューから [CORS を有効にする] を選択します。

これか。

![CORS を有効に](mod3-create-REST-API/figs/fig-api-gateway-activate-CORS.png)

あとはだいたい書いてある通り。

### 実装: API をデプロイする

> 1. [アクション] ドロップダウンリストで、[Deploy API] を選択します。

これですね。

![Deploy API メニュー](mod3-create-REST-API/figs/fig-api-gateway-api-deploy-menu.png)

> 2. [デプロイステージ] ドロップダウンリストから [新しいステージ] を選択します。
> 2. [ステージ名] に「dev」と入力します。

こんな感じかな。

![「APIのデプロイ」ダイアログの設定内容](mod3-create-REST-API/figs/fig-api-gateway-api-deploy-setting.png)

> 5. [URL を呼び出す] の横にある URL をコピーして保存します (モジュール 5 で必要になります)。

こいつのことらしい。

![URLの呼び出し](mod3-create-REST-API/figs/fig-api-gateway-API-URL.png)

URL:
```
https://8ks7kzg0b7.execute-api.us-west-2.amazonaws.com/dev
```
