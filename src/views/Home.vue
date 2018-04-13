<template>
  <div class="home">
    <a class="button is-primary">Large</a>
    {{ JSON.stringify(firmwares)}}
  </div>
</template>

<script>
import { mapActions, mapState } from 'vuex';

export default {
  name: 'home',
  data: () => ({}),
  created() {
    this.$http.getPayloads(result => {
      const payloads = [];

      result.forEach(p => {
        let key = Object.keys(p)[0];
        let name = key.replace(/\/payload_/gi, '').replace(/.js/gi, '');

        const fw = name.substr(0, 4);
        const sizeKb = p[key];
        const size = `${(sizeKb / 1024).toFixed(2)}Kb`;

        key = btoa(key);
        name = name.substring(5).toUpperCase();

        payloads.push({
          key,
          fw,
          name,
          sizeKb,
          size,
        });
      });

      this.storePayloads(payloads);
    });
  },
  methods: {
    ...mapActions(['storePayloads']),
  },
  computed: {
    ...mapState(['payloads']),
    firmwares() {
      return this.payloads
        .map(p => p.fw)
        .reduce((x, y) => (x.includes(y) ? x : [...x, y]), []);
    },
  },
};
</script>
