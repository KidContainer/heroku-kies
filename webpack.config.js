var path = require("path");
const {CleanWebpackPlugin} = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");


module.exports = {
    mode: 'production',
    entry: './index.js',
    output: {
        path: path.resolve(__dirname, 'www'),
        filename: 'js/[name].[contenthash].js',
        publicPath: "/"
    },
    plugins: [
        new MiniCssExtractPlugin({
            filename: "css/[name].[contenthash].css"
        }),
        new HtmlWebpackPlugin({
            template: "./template/html/index.html",
            filename: "html/index.html"
        }),
        new HtmlWebpackPlugin({
            template: "./template/html/404_not_found.html",
            filename: "html/404_not_found.html"
        }),
        new CleanWebpackPlugin(),
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