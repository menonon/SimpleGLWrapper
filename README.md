# Simple GL Wrapper
Simple GL Wrapperは、個人的な、そこそこモダンなOpenGL/GLSL学習用の、C++簡易ラッパークラス群です。

各種Webサイトや以下の書籍を参考に学習/開発しています。

* OpenGL SuperBible Sixth Edition
* OpenGL Insights
* OpenGL 4 Shading Language Cookbook Second Edition

2013年頃にざっと書いて、あとはたまにちびちび更新しています。


## License

MIT License

## Notice

OpenGLの全機能を網羅しているわけではありませんし、必ずしも最新の機能を使用できているわけではないと思います。
しかし少なくともOpenGL 3.2以降、できるだけOpenGL 4.0以降の機能を使用するように心がけています。

C++についてはC++11以降などの最新の機能を使えておらず、古臭くあるいはBetterC的に見えるかもしれません。

## Dependencies

* OpenGL関連ライブラリ
    * GLEW
        * 2.1.0
    * GLFW
        * モダンでクロスプラットフォームなOpenGLライブラリ/ツールキットです
        * サンプルで使用しています
        * 3.2.1
* その他のOpenGL関連ライブラリ
    * glm
        * OpenGLと親和性の高い数学ライブラリです
        * 0.9.8.5
    * DevIL
        * OpenGLと親和性の高い画像IOライブラリです
        * Textureクラスで使用しています
        * 1.8
* その他のライブラリ
    * boost
        * C++の高機能なライブラリ群です
        * regexとfilesystemをTextureクラスで使用しています
        * 1.66.0
    * ffmpeg
        * 動画関連のライブラリです
        * サンプルSkySphereで使用しています
        * 3.4.2

バージョン番号は参考です。基本的には最新版で構わないはずですが、動作しない場合はバージョンを揃えてみてください。

サンプルのビルドシステムには CMake を採用しています。
サンプルのCMakeLists.txtは、ルート階層の3rdpartyフォルダ内に各ライブラリを配置していることを前提に記述されていますので、環境に合わせて記述を変更してください。
サンプルの実行時、Windowsの場合DLLをexeと同じ場所に配置するのを忘れないようにしてください。Linux等の場合LD_LIBRARY_PATH環境変数を忘れないようにしてください。

## Features

クラスなどについて、簡単に説明します。

### 基本機能

#### error関数群

_error.hpp, error.cpp_

エラー情報に関する関数群です。

OpenGLのエラー情報を標準出力に出力します。
OpenGL 4.3以降では高機能なデバッグ情報を出力できます。

#### Geometryクラス

_geometry.hpp, geometry.cpp_

頂点情報に関するクラスです。

位置、法線、テクスチャ座標、インデックスやプリミティブモードなどの情報をまとめて管理できます。

上記の情報をセットした後は、使いたい時にbind()するだけで使用できます。

#### Shaderクラス

_shader.hpp, shader.cpp_

シェーダーに関するクラスです。

シェーダーステージとファイル名を与えてコンパイルし、用途に応じて追加の設定を行った後、リンクをします。

使いたい時にuse()するだけで使用できます。
Uniform変数に値をセットする機能もあります。

#### Textureクラス

_texture.hpp_


画像ファイルを読み込んで、テクスチャとして設定するクラスです。

データの型(GLfloatなど)のテンプレートになっています。
2Dテクスチャだけでなく3Dテクスチャなどの全テクスチャタイプに対応しています。

3Dテクスチャの指定を簡単にするために、与えるファイルネームが正規表現になっていますので注意が必要です。

使いたい時にbind()するだけで使用できます。

画像の読み込みにDevILを使用しています。
正規表現の処理にBoostのregexを使用しています。
ファイルパスの処理にBoostのfilesystemを使用しています。


#### VAOクラス

Vertex Array Objectのクラスです。

準備後、使いたい時にbind()するだけで使用できます。

### 応用機能

基本機能を使用した応用機能です。

#### MarchingCubesクラス

_marchingcubes.hpp, marchingcubes.cpp_

3Dテクスチャから等値面を生成して表示するためのクラスです。

Geometryシェーダーを使用しています。

しきい値に変更がなくても毎フレーム計算しているため、常に処理が重いです。

#### MarchingCubesTFクラス

_marchingcubes_tf.hpp, marchingcubes_tf.cpp_

上記のMarchingCubesクラスをTransform Feedbackを使用するように改造したクラスです。

しきい値を変更している間は重くなりますが、しきい値に変更のない場合は軽快に動作します。

参考までに垂直同期を切って簡単にフレームレートを計測すると、しきい値を毎フレーム変更している場合は30fps前後、しきい値を変更しない場合は100~1600fps(そのしきい値で生成されるポリゴン数によります)程度で動作します。(GeForce GTX 1080を使用　データサイズ:256x256x256)

## Samples

基本機能、応用機能を使用したサンプルです。

サンプルで使用しているシェーダーファイルはshaderフォルダ内にあります。
サンプルで使用している画像ファイルはdataフォルダ内にあります。

サンプルはCMakeを使ってビルド(に必要なファイルを用意)できます。

### tex3d

テクスチャを使用したサンプルです。

2Dテクスチャを表示してみる機能と3Dテクスチャを表示してみる機能、さらに3DテクスチャをMarchingCubesを用いて表示する機能のサンプルです。

### tf

tex3dのMarchingCubesをMarchingCubesTFに置き換えたバージョンです。

### tess

Tessellation Control/Evaluationシェーダーを使用した簡単で単純なサンプルです。

### SkySphere

よそからの要求があって作成した、UVスフィアにテクスチャを張るサンプルです。

動画の読み込みにも対応しています。(音声の再生には非対応)