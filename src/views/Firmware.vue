<template>
  <div class="firmwares">
    <h1>{{ id }}</h1>
    <router-link class="back" to="/">BACK</router-link>
    <div class="clearfix">
      <a v-for="p in availablePayloads" :key="p.key" class="button pull-left" :href="`${hostAddress}entry/{p.key}`">{{ p.name }}</a>
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';
export default {
  name: 'firmwares',
  props: {
    id: String,
  },
  computed: {
    ...mapState(['payloads']),
    currentFirmware() {
      return this.id;
    },
    availablePayloads() {
      return this.payloads.filter(f => f.fw === this.id);
    },
    hostAddress() {
      if (process.env.NODE_ENV === 'production') {
        return '/';
      } else {
        return 'http://192.168.0.235/';
      }
    },
  },
};
</script>
