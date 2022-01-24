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
