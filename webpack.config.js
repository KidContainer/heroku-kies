var path = require("path");
const {CleanWebpackPlugin} = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");


module.exports = {
    mode: 'production',
    entry: {
        "main":'./index.js',
        "404_not_found":"./404_not_found.js"
    },
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
            template: "./src/html/index.html",
            filename: "html/index.html",
            chunks: ["main"]
        }),
        new HtmlWebpackPlugin({
            template: "./src/html/404_not_found.html",
            filename: "html/404_not_found.html",
            chunks: ["404_not_found"]
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