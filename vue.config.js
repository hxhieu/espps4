module.exports = {
  devServer: {
    proxy: {
      '/api': {
        target: 'http://192.168.0.235',
        ws: true,
        changeOrigin: true,
      },
    },
  },
  chainWebpack: config => {
    if (process.env.NODE_ENV === 'production') {
      config.devtool(false);
      // Update the output files without the random hash so we can include in VS builds
      // JS
      config.output
        .publicPath('')
        .filename(`[name].[chunkhash:8].js`)
        .chunkFilename(`[id].[chunkhash:8].js`);

      // CSS
      config.plugin('extract-css').tap(args => {
        var opts = args[0];
        return Object.assign([], args, {
          0: {
            ...opts,
            filename: '[name].[contenthash:8].css',
          },
        });
      });
    }
  },
};
