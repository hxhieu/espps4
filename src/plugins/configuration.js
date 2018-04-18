export default {
  install(instance) {
    instance.prototype.$config = {
      host: () =>
        // eslint-disable-next-line
        process.env.NODE_ENV === 'production' ? '/' : 'http://192.168.0.235/',
    };
  },
};
