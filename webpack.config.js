var path = require("path");
const {CleanWebpackPlugin} = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");


module.exports = {
    mode: 'development',
    entry: './src/index.js',
    output: {
        path: path.resolve(__dirname, 'www/dist'),
        filename: '[name].[contenthash].js',
        publicPath: "/"
    },
    plugins: [
        new CleanWebpackPlugin(),
        new HtmlWebpackPlugin({
            template: "./src/template/html/template/index.html",
            filename: "template/html/index.html"
        }),
        new HtmlWebpackPlugin({
            template: "./src/template/html/template/404_not_found.html",
            filename: "template/html/404_not_found.html"
        }),
        new MiniCssExtractPlugin({
            filename: "www/css/[name].[contenthash].js"
        }),
    ],
    module: {
        rules: [
            {
                test: /\.css$/,
                use: [MiniCssExtractPlugin.loader, "css-loader"]
            }
        ]
    }
};